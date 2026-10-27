
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>

#include "../shared.h"



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
