#include "GlyphRasterCache.h"

#include <CoreLabs/io/File.h>
#include <CoreLabs/io/ByteReader.h>
#include <CoreLabs/io/ByteWriter.h>
#include <CoreLabs/io/ReadableMemory.h>
#include <CoreLabs/io/WritableMemory.h>
#include <CoreLabs/text/format.h>
#include <CoreLabs/logging.h>

#include <filesystem>
#include <cstdint>

#define ZLIB_CONST
#define ZLIB_WINAPI
#include <zlib.h>



namespace fl7::fonts::raster {



namespace {

    // Bump this if the on-disk blob layout below changes: a mismatched version
    // makes `try_load` treat the entry as a miss, regardless of the cache key.
    constexpr uint32_t CACHE_FORMAT_VERSION = 1;

    // version + pixel_format + channel_order + width + height + depth + left + top (8 x uint32_t) + data length (uint64_t).
    constexpr size_t FIXED_HEADER_SIZE = 8 * sizeof(uint32_t) + sizeof(uint64_t);
    constexpr size_t CHECKSUM_SIZE = sizeof(uint32_t);

    uint32_t _checksum(cl7::byte_view data)
    {
        uLong crc = ::crc32(0L, Z_NULL, 0);
        crc = ::crc32(crc, reinterpret_cast<const Bytef*>(data.data()), static_cast<uInt>(data.size()));
        return static_cast<uint32_t>(crc);
    }

} // namespace



    /**
     * Constructs a cache backed by the given directory, creating it (and any
     * missing parent directories) if it doesn't exist yet.
     */
    GlyphRasterCache::GlyphRasterCache(cl7::u8string_view cache_directory)
        : _cache_directory(cache_directory)
    {
        if (_cache_directory.empty() || _cache_directory.back() != u8'/')
            _cache_directory.push_back(u8'/');

        std::error_code error_code;
        std::filesystem::create_directories(std::filesystem::path(_cache_directory), error_code);
    }



    /**
     * Attempts to load a previously cached rasterization result for the given key.
     * Returns `std::nullopt` on a cache miss, or if the cached entry is missing or
     * corrupt (this never throws or crashes on bad data: a corrupt entry is just
     * treated as a miss).
     */
    std::optional<RasterResult> GlyphRasterCache::try_load(size_t key) const
    {
        cl7::io::File file(_get_file_path(key), cl7::io::OpenMode::Read);
        if (!file.is_readable())
            return std::nullopt; // Not necessarily an error, likely just a cache miss.

        cl7::io::ByteReader file_reader{&file};
        const cl7::byte_vector blob = file_reader.read_all();

        if (blob.size() < FIXED_HEADER_SIZE + CHECKSUM_SIZE)
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" is truncated. Treating it as a miss.");
            return std::nullopt;
        }

        cl7::io::ReadableMemory memory{cl7::byte_view(blob)};
        cl7::io::ByteReader reader{&memory};

        const auto format_version = reader.read_scalar<uint32_t>();
        const auto pixel_format = reader.read_scalar<uint32_t>();
        const auto channel_order = reader.read_scalar<uint32_t>();
        const auto width = reader.read_scalar<uint32_t>();
        const auto height = reader.read_scalar<uint32_t>();
        const auto depth = reader.read_scalar<uint32_t>();
        const auto left = reader.read_scalar<int32_t>();
        const auto top = reader.read_scalar<int32_t>();
        const auto data_size = reader.read_scalar<uint64_t>();

        if (format_version != CACHE_FORMAT_VERSION)
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" has an outdated format version. Treating it as a miss.");
            return std::nullopt;
        }

        if (data_size != blob.size() - FIXED_HEADER_SIZE - CHECKSUM_SIZE)
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" has an inconsistent data size. Treating it as a miss.");
            return std::nullopt;
        }

        cl7::byte_vector pixel_data(data_size);
        if (reader.read_bytes(cl7::make_byte_span(pixel_data)) != data_size)
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" could not be fully read. Treating it as a miss.");
            return std::nullopt;
        }

        const auto stored_checksum = reader.read_scalar<uint32_t>();
        const auto expected_checksum = _checksum(cl7::byte_view(blob).subspan(0, blob.size() - CHECKSUM_SIZE));
        if (stored_checksum != expected_checksum)
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" failed its checksum check. Treating it as a miss.");
            return std::nullopt;
        }

        xl7::graphics::images::ImageDesc desc{
            .pixel_format = static_cast<xl7::graphics::PixelFormat>(pixel_format),
            .channel_order = static_cast<xl7::graphics::ChannelOrder>(channel_order),
            .width = width,
            .height = height,
            .depth = depth,
        };

        if (desc.calculate_data_size() != pixel_data.size())
        {
            LOG_WARNING(u8"Glyph raster cache entry " + _get_filename(key) + u8" has an invalid image descriptor's data size. Treating it as a miss.");
            return std::nullopt;
        }

        return RasterResult{
            .glyph_image = xl7::graphics::images::Image(desc, std::move(pixel_data)),
            .pixel_offset = {.left = left, .top = top},
        };
    }

    /**
     * Stores a rasterization result under the given key, overwriting any existing
     * entry. Failures are logged and signaled via the return value but otherwise
     * ignored, since caching is a pure optimization and must never break rendering.
     */
    bool GlyphRasterCache::store(size_t key, const RasterResult& result) const
    {
        cl7::io::WritableMemory memory;
        cl7::io::ByteWriter writer{&memory};

        writer.write_scalar(CACHE_FORMAT_VERSION);
        writer.write_scalar(static_cast<uint32_t>(result.glyph_image.get_pixel_format()));
        writer.write_scalar(static_cast<uint32_t>(result.glyph_image.get_channel_order()));
        writer.write_scalar(static_cast<uint32_t>(result.glyph_image.get_width()));
        writer.write_scalar(static_cast<uint32_t>(result.glyph_image.get_height()));
        writer.write_scalar(static_cast<uint32_t>(result.glyph_image.get_depth()));
        writer.write_scalar(static_cast<int32_t>(result.pixel_offset.left));
        writer.write_scalar(static_cast<int32_t>(result.pixel_offset.top));
        writer.write_scalar(static_cast<uint64_t>(result.glyph_image.get_data().size()));
        writer.write_bytes(result.glyph_image.get_data());

        writer.write_scalar(_checksum(memory.get_data()));

        cl7::io::File file(_get_file_path(key), cl7::io::OpenMode::Truncate);
        if (!file.is_writable())
        {
            LOG_WARNING(u8"Could not write glyph raster cache entry " + _get_filename(key) + u8" (target file not writable).");
            return false;
        }

        if (file.write(memory.get_data()) != memory.get_data().size())
        {
            LOG_WARNING(u8"Could not fully write glyph raster cache entry " + _get_filename(key) + u8".");
            return false;
        }

        return true;
    }



    cl7::u8string GlyphRasterCache::_get_file_path(size_t key) const
    {
        return _cache_directory + _get_filename(key);
    }

    cl7::u8string GlyphRasterCache::_get_filename(size_t key)
    {
        return cl7::u8string(cl7::text::format::to_hex_lc(key, 16)) + u8".glyphcache";
    }



} // namespace fl7::fonts::raster
