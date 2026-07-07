
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>
#include <FaceLabs/fonts/raster/AnalyticalCoverageRasterizer.h>

#include <TestLabs/TestSuite.h>

#include <CoreLabs/platform/filesystem.h>

#include <iostream>



namespace {

    void dump_glyph_ascii(fl7::fonts::detail::ttf::TrueTypeFontLoader& loader, char ch, float font_size)
    {
        fl7::fonts::raster::AnalyticalCoverageRasterizer rasterizer;

        const cl7::text::codec::codepoint codepoint{.value = static_cast<unsigned>(ch)};
        const fl7::fonts::Glyph glyph = loader.load_glyph(codepoint);

        const auto raster_result = rasterizer.rasterize_glyph(glyph, {.font_size = font_size, .padding = 1});
        const auto& image = raster_result.glyph_image;

        std::cout << "Glyph '" << ch << "' @ " << font_size << "px (" << image.get_width() << "x" << image.get_height() << "):\n";

        static constexpr const char* ramp = " .:-=+*#%@";
        const auto* data = reinterpret_cast<const uint8_t*>(image.get_data().data());

        for (unsigned y = 0; y < image.get_height(); ++y)
        {
            std::string line;
            line.reserve(image.get_width());
            for (unsigned x = 0; x < image.get_width(); ++x)
            {
                const uint8_t v = data[static_cast<size_t>(y) * image.get_width() + x];
                const size_t idx = (static_cast<size_t>(v) * 9 + 127) / 255;
                line += ramp[idx];
            }
            std::cout << line << '\n';
        }
        std::cout << std::endl;
    }

} // namespace

TESTLABS_CASE(u8"scratch:  FaceLabs:  raster inspection")
{
    fl7::fonts::detail::ttf::TrueTypeFontLoader loader(cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf");

    for (char ch : {'8', '9', 'B', 'Q', 'W', 'e', 'g', 'o', 'y'})
    {
        //dump_glyph_ascii(loader, ch, 16.0f);
    }
}
