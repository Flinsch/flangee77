
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>
#include <FaceLabs/fonts/raster/SimpleBitmapRasterizer.h>
#include <FaceLabs/fonts/raster/OversampledBitmapRasterizer.h>
#include <FaceLabs/fonts/raster/AnalyticalCoverageRasterizer.h>
#include <FaceLabs/fonts/raster/SdfRasterizer.h>
#include <FaceLabs/fonts/raster/MsdfRasterizer.h>

#include <TestLabs/TestSuite.h>

#include <CoreLabs/platform/filesystem.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>



namespace {

    cl7::text::codec::codepoint to_codepoint(char ch)
    {
        return {.value = static_cast<unsigned>(ch)};
    }

    void dump_ascii(const std::string& title, const xl7::graphics::images::Image& image, unsigned channel_offset, unsigned channel_count)
    {
        static constexpr const char* ramp = " .:-=+*#%@";
        const auto* data = reinterpret_cast<const uint8_t*>(image.get_data().data());
        const unsigned bytes_per_pixel = image.get_desc().determine_bytes_per_pixel();

        uint8_t min_v = 255;
        uint8_t max_v = 0;

        std::vector<std::string> lines;
        for (unsigned y = 0; y < image.get_height(); ++y)
        {
            std::string line;
            line.reserve(image.get_width());
            for (unsigned x = 0; x < image.get_width(); ++x)
            {
                const uint8_t* pixel = data + (static_cast<size_t>(y) * image.get_width() + x) * bytes_per_pixel;
                unsigned sum = 0;
                for (unsigned c = 0; c < channel_count; ++c)
                    sum += pixel[channel_offset + c];
                const auto v = static_cast<uint8_t>(sum / channel_count);
                min_v = (std::min)(min_v, v);
                max_v = (std::max)(max_v, v);
                const size_t idx = (static_cast<size_t>(v) * 9 + 127) / 255;
                line += ramp[idx];
            }
            lines.push_back(std::move(line));
        }

        std::cout << title << " (" << image.get_width() << "x" << image.get_height() << ", min=" << static_cast<int>(min_v) << " max=" << static_cast<int>(max_v) << "):\n";
        for (const auto& line : lines)
            std::cout << line << '\n';
        std::cout << std::endl;
    }

    void dump_median_ascii(const std::string& title, const xl7::graphics::images::Image& image)
    {
        static constexpr const char* ramp = " .:-=+*#%@";
        const auto* data = reinterpret_cast<const uint8_t*>(image.get_data().data());
        const unsigned bytes_per_pixel = image.get_desc().determine_bytes_per_pixel();

        uint8_t min_v = 255;
        uint8_t max_v = 0;

        std::vector<std::string> lines;
        for (unsigned y = 0; y < image.get_height(); ++y)
        {
            std::string line;
            line.reserve(image.get_width());
            for (unsigned x = 0; x < image.get_width(); ++x)
            {
                const uint8_t* pixel = data + (static_cast<size_t>(y) * image.get_width() + x) * bytes_per_pixel;
                const uint8_t r = pixel[0];
                const uint8_t g = pixel[1];
                const uint8_t b = pixel[2];
                const uint8_t med = (std::max)((std::min)(r, g), (std::min)((std::max)(r, g), b));
                min_v = (std::min)(min_v, med);
                max_v = (std::max)(max_v, med);
                const size_t idx = (static_cast<size_t>(med) * 9 + 127) / 255;
                line += ramp[idx];
            }
            lines.push_back(std::move(line));
        }

        std::cout << title << " (" << image.get_width() << "x" << image.get_height() << ", min=" << static_cast<int>(min_v) << " max=" << static_cast<int>(max_v) << "):\n";
        for (const auto& line : lines)
            std::cout << line << '\n';
        std::cout << std::endl;
    }

    void dump_simple_glyph(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::SimpleBitmapRasterizer rasterizer;
        const fl7::fonts::Glyph glyph = loader.load_glyph(to_codepoint(ch));
        const auto result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 1});
        dump_ascii(std::string("Simple '") + ch + "'", result.glyph_image, 0, 1);
    }

    void dump_oversampled_glyph(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::OversampledBitmapRasterizer rasterizer(2);
        const fl7::fonts::Glyph glyph = loader.load_glyph(to_codepoint(ch));
        const auto result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 1});
        dump_ascii(std::string("Oversampled '") + ch + "'", result.glyph_image, 0, 1);
    }

    void dump_coverage_glyph(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::AnalyticalCoverageRasterizer rasterizer;
        const fl7::fonts::Glyph glyph = loader.load_glyph(to_codepoint(ch));
        const auto result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 1});
        dump_ascii(std::string("Coverage '") + ch + "'", result.glyph_image, 0, 1);
    }

    void dump_sdf_glyph(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::SdfRasterizer rasterizer;
        const fl7::fonts::Glyph glyph = loader.load_glyph(to_codepoint(ch));
        const auto result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 4});
        dump_ascii(std::string("SDF '") + ch + "'", result.glyph_image, 0, 1);
    }

    void dump_msdf_glyph(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::MsdfRasterizer rasterizer;
        const fl7::fonts::Glyph glyph = loader.load_glyph(to_codepoint(ch));
        const auto result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 4});
        dump_ascii(std::string("MSDF R '") + ch + "'", result.glyph_image, 0, 1);
        dump_ascii(std::string("MSDF G '") + ch + "'", result.glyph_image, 1, 1);
        dump_ascii(std::string("MSDF B '") + ch + "'", result.glyph_image, 2, 1);
        dump_median_ascii(std::string("MSDF median '") + ch + "'", result.glyph_image);
    }

} // namespace

TESTLABS_CASE(u8"scratch:  FaceLabs:  raster inspection")
{
    fl7::fonts::detail::ttf::TrueTypeFontLoader loader(cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf");
    //fl7::fonts::detail::ttf::TrueTypeFontLoader loader(cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSerif-Regular.ttf");

    for (char ch : {'8', '9', 'A', 'B', 'Q', 'V', 'W', 'e', 'g', 'k', 'o', 'y'})
    {
        //dump_simple_glyph(loader, ch, 16.0f);
        //dump_oversampled_glyph(loader, ch, 16.0f);
        //dump_coverage_glyph(loader, ch, 16.0f);
        //dump_sdf_glyph(loader, ch, 16.0f);
        //dump_msdf_glyph(loader, ch, 16.0f);
    }
}
