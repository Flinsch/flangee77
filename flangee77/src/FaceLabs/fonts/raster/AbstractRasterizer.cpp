#include "AbstractRasterizer.h"



namespace fl7::fonts::raster {



    /**
     * Rasterizes a glyph into an image and calculates its positioning offset. The
     * font size to be used for rasterization is specified in pixels. Additional
     * padding (in pixels) can be applied around the glyph during rasterization.
     */
    AbstractRasterizer::Result AbstractRasterizer::rasterize_glyph(const Glyph& glyph, float font_size, unsigned padding)
    {
        ml7::Vector2f lower_left = glyph.bounding_box.lower_left * font_size;
        ml7::Vector2f upper_right = glyph.bounding_box.upper_right * font_size;

        const float min_x = std::floor(lower_left.x);
        const float min_y = std::floor(-upper_right.y);
        const float max_x = std::ceil(upper_right.x);
        const float max_y = std::ceil(-lower_left.y);

        const int min_x_px = static_cast<int>(min_x) - static_cast<int>(padding);
        const int min_y_px = static_cast<int>(min_y) - static_cast<int>(padding);
        const int max_x_px = static_cast<int>(max_x) + static_cast<int>(padding);
        const int max_y_px = static_cast<int>(max_y) + static_cast<int>(padding);

        assert(max_x_px > min_x_px);
        assert(max_y_px > min_y_px);
        const auto pixel_width = static_cast<unsigned>(max_x_px - min_x_px);
        const auto pixel_height = static_cast<unsigned>(max_y_px - min_y_px);

        Offset offset;
        offset.top = min_y_px;
        offset.left = min_x_px;

        xl7::graphics::images::Image::Desc desc;
        desc.pixel_format = get_pixel_format();
        desc.channel_order = get_channel_order();
        desc.width = pixel_width;
        desc.height = pixel_height;
        desc.depth = 1;

        const unsigned pixel_stride = desc.determine_pixel_stride();
        const size_t data_size = desc.calculate_data_size();

        cl7::byte_vector data{data_size};

        dl7::Buffer2dSpan canvas{data, pixel_width, pixel_height, pixel_stride, 0};

        _rasterize_glyph_into(glyph, font_size, padding, offset, canvas);

        return {
            .image = {desc, std::move(data)},
            .offset = offset,
        };
    }

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset. The font size to be used for rasterization is specified in
     * pixels. Additional padding (in pixels) can be applied around the glyph during
     * rasterization.
     */
    void AbstractRasterizer::rasterize_glyph_into(const Glyph& glyph, float font_size, unsigned padding, const Offset& offset, dl7::Buffer2dSpan canvas)
    {
        _rasterize_glyph_into(glyph, font_size, padding, offset, canvas);
    }



} // namespace fl7::fonts::raster
