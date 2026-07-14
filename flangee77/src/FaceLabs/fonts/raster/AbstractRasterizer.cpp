#include "AbstractRasterizer.h"

#include <CoreLabs/bits.h>

#include <bit>



namespace fl7::fonts::raster {



namespace {

    // Bump this if the fields folded into `_compute_cache_key` below change.
    constexpr uint32_t CACHE_KEY_VERSION = 1;

    void _hash_combine_float(size_t& hash, float value)
    {
        cl7::bits::hash_combine(hash, static_cast<size_t>(std::bit_cast<uint32_t>(value)));
    }

} // namespace



    /**
     * Constructs an abstract rasterizer with the specified pixel format for the
     * output images and an (optional) channel order (which defaults to RGBA).
     */
    AbstractRasterizer::AbstractRasterizer(xl7::graphics::PixelFormat pixel_format, xl7::graphics::ChannelOrder channel_order) noexcept
        : _pixel_format(pixel_format)
        , _channel_order(channel_order)
    {
    }



    /**
     * Rasterizes a glyph into an image and calculates its positioning offset.
     * Uses the glyph cache set via `set_cache`, if any.
     */
    RasterResult AbstractRasterizer::rasterize_glyph(const Glyph& glyph, const RasterSizeConfig& size_config)
    {
        size_t cache_key = 0;
        if (_cache)
        {
            cache_key = _compute_cache_key(glyph, size_config);
            if (std::optional<RasterResult> cached = _cache->try_load(cache_key))
                return std::move(*cached);
        }

        ml7::Vector2f lower_left = glyph.bounding_box.lower_left * size_config.font_size;
        ml7::Vector2f upper_right = glyph.bounding_box.upper_right * size_config.font_size;

        const float min_x = std::floor(lower_left.x);
        const float min_y = std::floor(-upper_right.y);
        const float max_x = std::ceil(upper_right.x);
        const float max_y = std::ceil(-lower_left.y);

        const int min_x_px = static_cast<int>(min_x) - static_cast<int>(size_config.padding);
        const int min_y_px = static_cast<int>(min_y) - static_cast<int>(size_config.padding);
        const int max_x_px = static_cast<int>(max_x) + static_cast<int>(size_config.padding);
        const int max_y_px = static_cast<int>(max_y) + static_cast<int>(size_config.padding);

        assert(max_x_px > min_x_px);
        assert(max_y_px > min_y_px);
        const auto width_px = static_cast<unsigned>(max_x_px - min_x_px);
        const auto height_px = static_cast<unsigned>(max_y_px - min_y_px);

        PixelOffset pixel_offset = {
            .left = min_x_px,
            .top = min_y_px,
        };

        xl7::graphics::images::ImageDesc desc;
        desc.pixel_format = get_pixel_format();
        desc.channel_order = get_channel_order();
        desc.width = width_px;
        desc.height = height_px;
        desc.depth = 1;

        const unsigned bytes_per_pixel = desc.determine_bytes_per_pixel();
        const size_t data_size = desc.calculate_data_size();

        cl7::byte_vector data{data_size};

        dl7::Buffer2dSpan canvas{data, width_px, height_px, bytes_per_pixel, 0};

        _rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);

        RasterResult result{
            .glyph_image = {desc, std::move(data)},
            .pixel_offset = pixel_offset,
        };

        if (_cache)
            _cache->store(cache_key, result);

        return result;
    }

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset. Never uses any glyph cache.
     */
    void AbstractRasterizer::rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        _rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);
    }



    /**
     * Computes an opaque cache key covering everything that determines this glyph's
     * rasterized output: the glyph's own outline geometry, the size config, this
     * rasterizer's pixel format and channel order, and (via `_cache_key_params_hash`)
     * any rasterizer-specific parameters.
     */
    size_t AbstractRasterizer::_compute_cache_key(const Glyph& glyph, const RasterSizeConfig& size_config) const
    {
        size_t hash = 0;
        cl7::bits::hash_combine(hash, static_cast<size_t>(CACHE_KEY_VERSION));
        cl7::bits::hash_combine(hash, static_cast<size_t>(glyph.codepoint.value));

        for (const Contour& contour : glyph.contours)
        {
            for (const ml7::Vector2f& point : contour.points)
            {
                _hash_combine_float(hash, point.x);
                _hash_combine_float(hash, point.y);
            }
        }

        _hash_combine_float(hash, glyph.bounding_box.lower_left.x);
        _hash_combine_float(hash, glyph.bounding_box.lower_left.y);
        _hash_combine_float(hash, glyph.bounding_box.upper_right.x);
        _hash_combine_float(hash, glyph.bounding_box.upper_right.y);

        _hash_combine_float(hash, size_config.font_size);
        cl7::bits::hash_combine(hash, static_cast<size_t>(size_config.padding));

        cl7::bits::hash_combine(hash, static_cast<size_t>(get_pixel_format()));
        cl7::bits::hash_combine(hash, static_cast<size_t>(get_channel_order()));

        cl7::bits::hash_combine(hash, _cache_key_params_hash());

        return hash;
    }



} // namespace fl7::fonts::raster
