#ifndef FL7_FONTS_RASTER_GLYPHRASTERCACHE_H
#define FL7_FONTS_RASTER_GLYPHRASTERCACHE_H

#include "./RasterResult.h"

#include <CoreLabs/string.h>
#include <CoreLabs/text/codec/codepoint.h>

#include <optional>



namespace fl7::fonts::raster {



/**
 * A disk-backed cache for rasterized glyph results, keyed by an opaque hash
 * (see `AbstractRasterizer`'s cache-key computation).
 *
 * Lets expensive rasterization (e.g., SDF/MSDF) be skipped on subsequent runs if a
 * glyph has already been rasterized once with the same inputs.
 */
class GlyphRasterCache
{

public:
    /**
     * Constructs a cache backed by the given directory, creating it (and any
     * missing parent directories) if it doesn't exist yet.
     *
     * The filename prefix, if given, is prepended to every entry's filename
     * verbatim (no separator is inserted automatically, include one yourself, e.g.,
     * "MSDF-NotoSans-Regular-", if you want one). Together with the codepoint
     * that's always part of the filename, this makes it possible to identify (and
     * selectively delete) a specific font/rasterizer/glyph's cache entries on disk,
     * instead of having to either wipe the whole cache directory or namespace it
     * via a directory hierarchy alone.
     */
    explicit GlyphRasterCache(cl7::u8string_view cache_directory, cl7::u8string_view filename_prefix = {});



    /**
     * Attempts to load a previously cached rasterization result for the given code
     * point and key. Returns `std::nullopt` on a cache miss, or if the cached entry
     * is missing or corrupt (this never throws or crashes on bad data: a corrupt
     * entry is just treated as a miss).
     */
    std::optional<RasterResult> try_load(cl7::text::codec::codepoint codepoint, size_t key) const;

    /**
     * Stores a rasterization result under the given code point and key, overwriting
     * any existing entry. Failures are logged and signaled via the return value but
     * otherwise ignored, since caching is a pure optimization and must never break
     * rendering.
     */
    bool store(cl7::text::codec::codepoint codepoint, size_t key, const RasterResult& result) const;



private:
    cl7::u8string _get_file_path(cl7::text::codec::codepoint codepoint, size_t key) const;

    cl7::u8string _get_filename(cl7::text::codec::codepoint codepoint, size_t key) const;

    cl7::u8string _cache_directory;
    cl7::u8string _filename_prefix;

}; // class GlyphRasterCache



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_GLYPHRASTERCACHE_H
