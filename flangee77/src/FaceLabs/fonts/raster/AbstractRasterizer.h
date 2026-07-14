#ifndef FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H

#include "./GlyphRasterCache.h"
#include "./RasterResult.h"
#include "./RasterSizeConfig.h"

#include "../Glyph.h"

#include <DataLabs/Buffer2d.h>



namespace fl7::fonts::raster {



/**
 * Abstract class for rasterizing glyphs into images, for example as a "classic"
 * bitmap, a signed distance field (SDF), or a multi-channel signed distance field
 * (MSDF), and (optionally) calculating the positional offset of a rasterized image
 * relative to the typographic origin of the glyph.
 */
class AbstractRasterizer
{

public:
    /**
     * Constructs an abstract rasterizer with the specified pixel format for the
     * output images and an (optional) channel order (which defaults to RGBA).
     */
    AbstractRasterizer(xl7::graphics::PixelFormat pixel_format, xl7::graphics::ChannelOrder channel_order = xl7::graphics::ChannelOrder::RGBA) noexcept;

    virtual ~AbstractRasterizer() noexcept = default;



    /**
     * Rasterizes a glyph into an image and calculates its positioning offset.
     * Uses the glyph cache set via `set_cache`, if any.
     */
    RasterResult rasterize_glyph(const Glyph& glyph, const RasterSizeConfig& size_config);

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset. Never uses any glyph cache.
     */
    void rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas);



    /**
     * Sets (or clears, via `nullptr`) a disk-backed cache to consult before
     * rasterizing a glyph, and to populate after rasterizing one. Not owned: the
     * caller must keep it alive for at least as long as this rasterizer. Disabled
     * (`nullptr`) by default, so this has zero effect on behavior or performance
     * unless explicitly opted into.
     */
    void set_cache(GlyphRasterCache* cache) noexcept { _cache = cache; }



    /**
     * Returns the pixel format of this rasterizer's output images.
     */
    xl7::graphics::PixelFormat get_pixel_format() const { return _pixel_format; }

    /**
     * Returns the channel order of this rasterizer's output images (defaults to RGBA).
     */
    xl7::graphics::ChannelOrder get_channel_order() const { return _channel_order; }



private:
    virtual void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) = 0;

    /**
     * Contributes rasterizer-specific parameters (and, by convention, an
     * algorithm-version constant that a subclass can bump to force-invalidate
     * previously cached results after an algorithmic change) to the cache key.
     * Rasterizers with no extra parameters can leave this at its default.
     */
    virtual size_t _cache_key_params_hash() const { return 0; }

    /**
     * Computes an opaque cache key covering everything that determines this glyph's
     * rasterized output: the glyph's own outline geometry, the size config, this
     * rasterizer's pixel format and channel order, and (via `_cache_key_params_hash`)
     * any rasterizer-specific parameters.
     */
    size_t _compute_cache_key(const Glyph& glyph, const RasterSizeConfig& size_config) const;

    /** The pixel format of this rasterizer's output images. */
    xl7::graphics::PixelFormat _pixel_format;
    /** The channel order of this rasterizer's output images (defaults to RGBA). */
    xl7::graphics::ChannelOrder _channel_order = xl7::graphics::ChannelOrder::RGBA;

    /** The (optional, not owned) disk cache consulted/populated by `rasterize_glyph`. */
    GlyphRasterCache* _cache = nullptr;

}; // class AbstractRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
