#include "OversampledBitmapRasterizer.h"

#include "./SimpleBitmapRasterizer.h"



namespace fl7::fonts::raster {



    /**
     * Constructs a bitmap rasterizer with the specified oversampling factor (e.g.,
     * (a factor of 4 effectively means 16 samples per pixel). A factor less than 2
     * disables oversampling and therefore also the actual anti-aliasing effect of
     * this rasterizer. Either way, a basic anti-aliasing quality can optionally be
     * set, which is then passed to the internally used simple binary rasterizer.
     * However, this only helps with the final result to a limited extent; one
     * cannot really speak of "quality" here.
     */
    OversampledBitmapRasterizer::OversampledBitmapRasterizer(unsigned oversampling, unsigned basic_aa_quality) noexcept
        : _oversampling(oversampling >= 2 ? oversampling : 1)
        , _basic_aa_quality(basic_aa_quality)
    {
    }



    void OversampledBitmapRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        if (_oversampling < 2)
        {
            _do_rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);
            return;
        }

        const auto samples_per_pixel = _oversampling * _oversampling;

        cl7::byte_vector oversampled_data(canvas.size() * samples_per_pixel);
        dl7::Buffer2dSpan oversampled_canvas(oversampled_data, canvas.width() * _oversampling, canvas.height() * _oversampling);

        const PixelOffset oversampled_pixel_offset = {
            .left = pixel_offset.left * static_cast<int>(_oversampling),
            .top = pixel_offset.top * static_cast<int>(_oversampling),
        };

        const RasterSizeConfig oversampled_size_config = {
            .font_size = size_config.font_size * static_cast<float>(_oversampling),
            .padding = size_config.padding * _oversampling,
        };

        _do_rasterize_glyph_into(glyph, oversampled_size_config, oversampled_pixel_offset, oversampled_canvas);

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                unsigned accumulated_value = 0;

                for (size_t y = 0; y < _oversampling; ++y)
                {
                    for (size_t x = 0; x < _oversampling; ++x)
                    {
                        accumulated_value += oversampled_canvas.map_element<uint8_t>(row * _oversampling + y, col * _oversampling + x);
                    }
                }

                unsigned average_value = (accumulated_value + samples_per_pixel / 2) / samples_per_pixel;
                assert(average_value <= 0xff);

                canvas.map_element<uint8_t>(row, col) = average_value;
            } // for each pixel column
        } // for each pixel row
    }



    void OversampledBitmapRasterizer::_do_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, const dl7::Buffer2dSpan& canvas)
    {
        SimpleBitmapRasterizer simple_bitmap_rasterizer{_basic_aa_quality, false};

        assert(simple_bitmap_rasterizer.get_pixel_format() == get_pixel_format());
        assert(simple_bitmap_rasterizer.get_channel_order() == get_channel_order());

        simple_bitmap_rasterizer.rasterize_glyph_into(glyph, size_config, pixel_offset, canvas);
    }



} // namespace fl7::fonts::raster
