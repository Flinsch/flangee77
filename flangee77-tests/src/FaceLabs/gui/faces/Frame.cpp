
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Frame.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  get_effective_preferred_size() is the content area's own preferred size plus the title bar height on top; get_effective_minimum_size() is the same against the content area's own minimum size instead, but never smaller than its own minimum width/height" )
{
    fl7::gui::faces::Frame frame;

    auto& child = frame.get_content_area().add_child<fl7::gui::Container>();
    child.set_preferred_size({100.0f, 50.0f});
    child.set_minimum_size({20.0f, 10.0f});

    TESTLABS_CHECK_EQ_FLT( frame.get_effective_preferred_size().x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( frame.get_effective_preferred_size().y, 74.0f ); // 50 + the title bar's own 24.

    // Content area's own minimum (20, 10) plus the title bar (24) would be
    // (20, 34), both below the minimum width (80)/height (40): clamped up to those instead.
    TESTLABS_CHECK_EQ_FLT( frame.get_effective_minimum_size().x, 80.0f );
    TESTLABS_CHECK_EQ_FLT( frame.get_effective_minimum_size().y, 40.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  get_effective_minimum_size() uses the content area's own minimum (plus the title bar) once that actually exceeds its own minimum width/height" )
{
    fl7::gui::faces::Frame frame;

    auto& child = frame.get_content_area().add_child<fl7::gui::Container>();
    child.set_minimum_size({100.0f, 60.0f}); // + the title bar (24) = (100, 84): both above the floor.

    TESTLABS_CHECK_EQ_FLT( frame.get_effective_minimum_size().x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( frame.get_effective_minimum_size().y, 84.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  get_title()/set_title()" )
{
    fl7::gui::faces::Frame frame(U"My Window");
    TESTLABS_CHECK_EQ( frame.get_title(), cl7::u32string(U"My Window") );

    frame.set_title(U"Renamed");
    TESTLABS_CHECK_EQ( frame.get_title(), cl7::u32string(U"Renamed") );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  a frame's own Style::padding never applies to the frame itself (see Face::_applies_own_padding()), though an explicit override still cascades down to its (otherwise themeless/padding-less) content area, same as to any other descendant" )
{
    fl7::gui::faces::Frame frame;

    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(50.0f); // Deliberately huge: would obviously show up below if it applied at all.
    frame.set_style_override(style);

    TESTLABS_CHECK_EQ_FLT( frame.get_effective_padding().left, 0.0f );
    TESTLABS_CHECK_EQ_FLT( frame.get_effective_padding().top, 0.0f );

    // The content area itself has no theme role of its own, so it falls
    // through to Frame's own explicit override instead (see
    // Face::_find_inherited_style_override()), and, being a plain,
    // unlayouted Container, actually applies that inherited padding to
    // itself; this is the pre-existing, generic override-cascading
    // behavior, not something Frame's own suppression touches at all.
    TESTLABS_CHECK_EQ_FLT( frame.get_content_area().get_effective_padding().left, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  get_content_area() is the only place a caller can actually add children; they become descendants of the content area, not the Frame itself" )
{
    fl7::gui::faces::Frame frame;

    auto& button = frame.get_content_area().add_child<fl7::gui::faces::Button>(U"Caption");
    TESTLABS_CHECK( button.get_parent() == &frame.get_content_area() );
    TESTLABS_CHECK( button.get_parent() != static_cast<fl7::gui::Face*>(&frame) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  the content area fills whatever's left below the title bar, and is kept in sync as the frame is resized" )
{
    fl7::gui::faces::Frame frame;

    // Position/size are only laid out once a resize actually triggers it (see
    // Frame::_on_size_changed()); a fresh frame hasn't had one yet.
    frame.set_size({200.0f, 150.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_position().y, 24.0f ); // The fixed title bar height.
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_size().x, 200.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_size().y, 126.0f ); // 150 minus the title bar.

    frame.set_size({100.0f, 80.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_size().x, 100.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_content_area().get_size().y, 56.0f ); // 80 minus the title bar.
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

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  dragging the resize handle resizes the frame by the drag delta" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({10.0f, 10.0f});
    frame.set_size({200.0f, 150.0f});

    // The resize handle is a fixed 16x16 square in the frame's own
    // bottom-right corner, i.e. absolute (194,144) to (210,160) here.
    input.move_mouse_to({200.0f, 150.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    input.move_mouse_to({230.0f, 180.0f}); // delta (30, 30)
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_size().x, 230.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, frame.get_size().y, 180.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  dragging the resize handle never shrinks the frame below its minimum size" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({10.0f, 10.0f});
    frame.set_size({200.0f, 150.0f});

    input.move_mouse_to({200.0f, 150.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    // Drags far enough up/left to try to shrink well past the minimum.
    input.move_mouse_to({0.0f, 0.0f});
    shell.update();
    TESTLABS_CHECK( frame.get_size().x >= 80.0f );
    TESTLABS_CHECK( frame.get_size().y >= 40.0f );

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

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Frame:  a button added to the content area is hit-tested/clicked correctly, offset by the title bar" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& frame = shell.add_face<fl7::gui::faces::Frame>(U"Title");
    frame.set_position({10.0f, 10.0f});
    frame.set_size({200.0f, 150.0f});

    auto& button = frame.get_content_area().add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({20.0f, 20.0f});
    button.set_size({80.0f, 24.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    // Button spans the frame's absolute (10+20, 10+24+20) to (10+100, 10+24+44):
    // offset both by the frame's own position and by the title bar on top of
    // the content area's own position.
    input.move_mouse_to({60.0f, 64.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( click_count, 1 );
}
