#ifndef F77_TESTS_FL7_FACELABS_SHARED_H
#define F77_TESTS_FL7_FACELABS_SHARED_H

#include <FaceLabs/fonts/Font.h>
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>

#include <CoreLabs/platform/filesystem.h>



namespace f77_tests::fonts {



    /**
     * A real, loaded font, shared by every test (fonts and gui alike) that needs
     * one: a font backend's own tests, text layout/raster tests, and any gui test
     * that needs get_effective_style().font to be non-null (e.g. via
     * set_style_override()). Always the same font, loaded once and kept alive for
     * the whole test run.
     */
    inline
    fl7::fonts::Font& get_test_font()
    {
        static fl7::fonts::Font font(std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(
            cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf"));
        return font;
    }



} // namespace f77_tests::fonts

#endif // F77_TESTS_FL7_FACELABS_SHARED_H
