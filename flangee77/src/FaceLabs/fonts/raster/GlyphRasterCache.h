#ifndef FL7_FONTS_RASTER_GLYPHRASTERCACHE_H
#define FL7_FONTS_RASTER_GLYPHRASTERCACHE_H

#include "./RasterResult.h"

#include <CoreLabs/string.h>

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
     */
    explicit GlyphRasterCache(cl7::u8string_view cache_directory);



    /**
     * Attempts to load a previously cached rasterization result for the given key.
     * Returns `std::nullopt` on a cache miss, or if the cached entry is missing or
     * corrupt (this never throws or crashes on bad data: a corrupt entry is just
     * treated as a miss).
     */
    std::optional<RasterResult> try_load(size_t key) const;

    /**
     * Stores a rasterization result under the given key, overwriting any existing
     * entry. Failures are logged and signaled via the return value but otherwise
     * ignored, since caching is a pure optimization and must never break rendering.
     */
    bool store(size_t key, const RasterResult& result) const;



private:
    cl7::u8string _get_file_path(size_t key) const;

    static cl7::u8string _get_filename(size_t key);

    cl7::u8string _cache_directory;

}; // class GlyphRasterCache



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_GLYPHRASTERCACHE_H
