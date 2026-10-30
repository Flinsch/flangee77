
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Frame.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  get_title()/set_title()" )
{
    fl7::gui::faces::Frame frame(U"My Window");
    TESTLABS_CHECK_EQ( frame.get_title(), cl7::u32string(U"My Window") );

    frame.set_title(U"Renamed");
    TESTLABS_CHECK_EQ( frame.get_title(), cl7::u32string(U"Renamed") );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  open()/close() toggle visibility, emitting get_opened()/get_closed() only on an actual change" )
{
    fl7::gui::faces::Frame frame;
    TESTLABS_CHECK( frame.is_visible() ); // starts open

    int opened_count = 0;
    int closed_count = 0;
    frame.get_opened().connect([&]() { ++opened_count; });
    frame.get_closed().connect([&]() { ++closed_count; });

    frame.open(); // already open: no-op
    TESTLABS_CHECK_EQ( opened_count, 0 );

    frame.close();
    TESTLABS_CHECK( !frame.is_visible() );
    TESTLABS_CHECK_EQ( closed_count, 1 );

    frame.close(); // already closed: no-op
    TESTLABS_CHECK_EQ( closed_count, 1 );

    frame.open();
    TESTLABS_CHECK( frame.is_visible() );
    TESTLABS_CHECK_EQ( opened_count, 1 );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  clicking the close button closes the frame" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({0.0f, 0.0f});
    frame.set_size({200.0f, 150.0f});

    int closed_count = 0;
    frame.get_closed().connect([&]() { ++closed_count; });

    // The close button is a fixed 24x24 square in the title bar's top-right corner.
    input.move_mouse_to({190.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK( !frame.is_visible() );
    TESTLABS_CHECK_EQ( closed_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  dragging the title bar moves the frame by the drag delta" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({10.0f, 10.0f});
    frame.set_size({200.0f, 150.0f});

    // Press within the title bar strip (local y < 24), clear of the close
    // button's rightmost 24px: the disabled title label lets the click fall
    // through to the Frame itself, arming the drag.
    input.move_mouse_to({60.0f, 20.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    input.move_mouse_to({90.0f, 50.0f}); // delta (30, 30)
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_position().x, 40.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_position().y, 40.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  pressing and dragging inside the content area does not move the frame" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({10.0f, 10.0f});
    frame.set_size({200.0f, 150.0f});

    // Below the title bar strip (local y >= 24): the hit resolves to the
    // (empty) content area instead of the Frame itself, so no dragging is
    // ever armed for it.
    input.move_mouse_to({60.0f, 80.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    input.move_mouse_to({90.0f, 110.0f});
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_position().x, 10.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_position().y, 10.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}
