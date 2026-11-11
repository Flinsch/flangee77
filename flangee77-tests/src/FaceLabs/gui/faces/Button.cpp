
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>

#include <CoreLabs/platform/filesystem.h>

#include "../shared.h"



namespace {

    /**
     * A real, loaded font, for the preferred/minimum-size tests below: those
     * need get_effective_style().font to be non-null, which set_style_override()
     * (bypassing the theme/Shell entirely) makes possible without a Shell at all.
     */
    fl7::fonts::Font& get_test_font()
    {
        static fl7::fonts::Font font(std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(
            cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf"));
        return font;
    }

} // namespace



TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  a click (press and release while hovered) fires get_clicked()" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"OK");
    button.set_position({0.0f, 0.0f});
    button.set_size({100.0f, 20.0f});

    int click_count = 0;
    button.get_clicked().connect([&click_count]() { ++click_count; });

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == &button );

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( shell.get_pressed_face() == &button );
    TESTLABS_CHECK( shell.get_focused_face() == &button );
    TESTLABS_CHECK_EQ( click_count, 0 );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_pressed_face() == nullptr );
    TESTLABS_CHECK_EQ( click_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  releasing outside the button (after being pressed inside it) does not click it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"OK");
    button.set_position({0.0f, 0.0f});
    button.set_size({100.0f, 20.0f});

    int click_count = 0;
    button.get_clicked().connect([&click_count]() { ++click_count; });

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( shell.get_pressed_face() == &button );

    // Mouse capture: the button stays the "pressed" face even once the pointer
    // has moved off of it, same as any other GUI (so a drag-away-to-cancel
    // gesture is actually possible), it just no longer counts as "hovered".
    input.move_mouse_to({500.0f, 500.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );
    TESTLABS_CHECK( shell.get_pressed_face() == &button );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_pressed_face() == nullptr );
    TESTLABS_CHECK_EQ( click_count, 0 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  a disabled button is neither hovered nor clickable" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"OK");
    button.set_position({0.0f, 0.0f});
    button.set_size({100.0f, 20.0f});
    button.set_enabled(false);

    int click_count = 0;
    button.get_clicked().connect([&click_count]() { ++click_count; });

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( click_count, 0 );
}



// #############################################################################
// Preferred/minimum size (text-based, same scheme as faces::Label)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  without an explicit override, get_preferred_size()/get_minimum_size() stay {0, 0} regardless of font; get_effective_preferred_size()/get_effective_minimum_size() need an effective font to calculate anything, else also {0, 0}" )
{
    fl7::gui::faces::Button button(U"OK");
    TESTLABS_CHECK_EQ_FLT( button.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_minimum_size().x, 0.0f );

    TESTLABS_CHECK_EQ_FLT( button.get_effective_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  set_style_override() with a real font: get_effective_preferred_size()/get_effective_minimum_size() both reflect the text's own natural (unwrapped) size, growing with more text; get_preferred_size()/get_minimum_size() stay {0, 0} regardless (no explicit override set)" )
{
    fl7::gui::Style style;
    style.font = &get_test_font();

    fl7::gui::faces::Button button(U"OK");
    button.set_style_override(style);

    const ml7::Vector2f short_size = button.get_effective_preferred_size();
    TESTLABS_CHECK( short_size.x > 0.0f );
    TESTLABS_CHECK( short_size.y > 0.0f );
    // A button's text can't usefully be laid out any smaller than its own natural size.
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().x, short_size.x );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().y, short_size.y );
    // No explicit override anywhere above: the plain getters stay untouched.
    TESTLABS_CHECK_EQ_FLT( button.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_minimum_size().x, 0.0f );

    button.set_text(U"Much Longer Caption");
    TESTLABS_CHECK( button.get_effective_preferred_size().x > short_size.x );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Button:  get_effective_preferred_size()/get_effective_minimum_size() add the effective style's own padding on top of the text's own natural size (minimum == preferred either way)" )
{
    fl7::gui::Style unpadded_style;
    unpadded_style.font = &get_test_font();

    fl7::gui::Style padded_style = unpadded_style;
    padded_style.padding = fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f); // left/right 4, top/bottom 2.

    fl7::gui::faces::Button unpadded_button(U"OK");
    unpadded_button.set_style_override(unpadded_style);
    fl7::gui::faces::Button padded_button(U"OK");
    padded_button.set_style_override(padded_style);

    const ml7::Vector2f unpadded = unpadded_button.get_effective_preferred_size();
    const ml7::Vector2f padded = padded_button.get_effective_preferred_size();

    TESTLABS_CHECK_EQ_FLT( padded.x, unpadded.x + 8.0f ); // + left(4) + right(4)
    TESTLABS_CHECK_EQ_FLT( padded.y, unpadded.y + 4.0f ); // + top(2) + bottom(2)
    TESTLABS_CHECK_EQ_FLT( padded_button.get_effective_minimum_size().x, padded.x );
    TESTLABS_CHECK_EQ_FLT( padded_button.get_effective_minimum_size().y, padded.y );
}
