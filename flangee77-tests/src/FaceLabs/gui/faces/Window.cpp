
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Window.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Window:  the content area fills the window's own size, staying at local position (0, 0), and is kept in sync as the window is resized" )
{
    fl7::gui::faces::Window window;
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_position().y, 0.0f );

    window.set_size({200.0f, 150.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_size().x, 200.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_size().y, 150.0f );

    window.set_size({100.0f, 80.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_size().x, 100.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, window.get_content_area().get_size().y, 80.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Window:  get_content_area() is the only place a caller can actually add children; they become descendants of the content area, not the Window itself" )
{
    fl7::gui::faces::Window window;

    auto& button = window.get_content_area().add_child<fl7::gui::faces::Button>(U"Caption");
    TESTLABS_CHECK( button.get_parent() == &window.get_content_area() );
    TESTLABS_CHECK( button.get_parent() != static_cast<fl7::gui::Face*>(&window) );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Window:  a button added to the content area is hit-tested/clicked right through the window, unoffset (the content area fills the window exactly)" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& window = shell.add_face<fl7::gui::faces::Window>();
    window.set_position({10.0f, 10.0f});
    window.set_size({200.0f, 150.0f});

    auto& button = window.get_content_area().add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({20.0f, 20.0f});
    button.set_size({80.0f, 24.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    // Button spans the window's absolute (10+20, 10+20) to (10+100, 10+44).
    input.move_mouse_to({60.0f, 30.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( click_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Window:  clicking outside the content area's bounds (but still within the window) hits the window itself, not the button" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& window = shell.add_face<fl7::gui::faces::Window>();
    window.set_position({0.0f, 0.0f});
    window.set_size({200.0f, 150.0f});

    auto& button = window.get_content_area().add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({20.0f, 20.0f});
    button.set_size({80.0f, 24.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    // Well clear of the button, but still inside the window/content area.
    input.move_mouse_to({150.0f, 100.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( click_count, 0 );
}
