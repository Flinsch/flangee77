#include "AbstractRasterizer.h"



namespace fl7::fonts::raster {



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
     */
    RasterResult AbstractRasterizer::rasterize_glyph(const Glyph& glyph, const RasterSizeConfig& size_config)
    {
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

        xl7::graphics::images::Image::Desc desc;
        desc.pixel_format = get_pixel_format();
        desc.channel_order = get_channel_order();
        desc.width = width_px;
        desc.height = height_px;
        desc.depth = 1;

        const unsigned pixel_stride = desc.determine_pixel_stride();
        const size_t data_size = desc.calculate_data_size();

        cl7::byte_vector data{data_size};

        dl7::Buffer2dSpan canvas{data, width_px, height_px, pixel_stride, 0};

        _rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);

        return {
            .glyph_image = {desc, std::move(data)},
            .pixel_offset = pixel_offset,
        };
    }

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset.
     */
    void AbstractRasterizer::rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        _rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);
    }



} // namespace fl7::fonts::raster
