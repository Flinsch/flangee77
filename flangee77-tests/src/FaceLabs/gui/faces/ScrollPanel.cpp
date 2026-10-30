
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/ScrollPanel.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  set_scroll_position() clamps to [0, content_size - viewport_size] per axis, emitting get_scroll_changed() only on an actual change" )
{
    fl7::gui::faces::ScrollPanel panel;
    panel.set_size({200.0f, 100.0f});
    panel.set_content_size({100.0f, 300.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_size().x, 100.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_size().y, 300.0f );

    int emit_count = 0;
    ml7::Vector2f last_position{};
    panel.get_scroll_changed().connect([&](ml7::Vector2f position) { ++emit_count; last_position = position; });

    // The vertical scroll bar is always shown by default, narrowing the
    // viewport to 184 x 100; its max scroll is therefore 300 - 100 = 200.
    panel.set_scroll_position({0.0f, 250.0f}); // above max: clamped
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 200.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ_ROUND( 3, last_position.y, 200.0f );

    // Horizontal content (100) doesn't overflow even the narrowed viewport
    // (184): its max scroll is 0, so any horizontal position is clamped away,
    // and since that's already where it was, this doesn't actually change
    // anything (no emit).
    panel.set_scroll_position({50.0f, 200.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().x, 0.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );

    panel.set_scroll_position({0.0f, 50.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 50.0f );
    TESTLABS_CHECK_EQ( emit_count, 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  set_vertical_scroll_enabled(false)/set_horizontal_scroll_enabled(false) clamp that axis's scroll position to 0 regardless of content size" )
{
    fl7::gui::faces::ScrollPanel panel;
    panel.set_size({200.0f, 100.0f});
    panel.set_content_size({300.0f, 300.0f});

    panel.set_vertical_scroll_enabled(false);
    TESTLABS_CHECK( !panel.get_vertical_scroll_enabled() );

    panel.set_scroll_position({50.0f, 150.0f});
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 0.0f );
    TESTLABS_CHECK( panel.get_scroll_position().x > 0.0f ); // horizontal is still enabled, unaffected
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  Auto visibility shows the horizontal scroll bar only once its content actually overflows, narrowing the content area accordingly" )
{
    fl7::gui::faces::ScrollPanel panel;
    panel.set_size({200.0f, 100.0f});

    // No content set yet (content size defaults to {0, 0}): the horizontal
    // bar (Auto) stays hidden, while the vertical one (Always) is shown
    // regardless, already narrowing the content area to 184px wide.
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_area().get_size().x, 184.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_area().get_size().y, 100.0f );

    panel.set_content_size({250.0f, 0.0f}); // overflows the full 200px width on its own
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_area().get_size().y, 84.0f ); // now narrowed by the horizontal bar too
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  Auto visibility's second layout pass also shows an axis that only overflows once the OTHER axis's scroll bar narrows its viewport" )
{
    fl7::gui::faces::ScrollPanel panel;
    panel.set_vertical_scroll_bar_visibility(fl7::gui::faces::ScrollPanel::ScrollBarVisibility::Auto);
    panel.set_size({200.0f, 100.0f});

    // Vertical overflows outright (150 > 100), so its bar is shown on the
    // first pass. Horizontal content (190) does NOT overflow the full 200px
    // width, but DOES overflow the 184px left once the vertical bar takes its
    // 16px: that only shows up on the second, interdependent pass.
    panel.set_content_size({190.0f, 150.0f});

    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_area().get_size().x, 184.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_content_area().get_size().y, 84.0f );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  hovering the content and scrolling the wheel scrolls vertically by get_wheel_step() per notch, clamped" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::ScrollPanel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({200.0f, 100.0f});
    panel.set_content_size({100.0f, 300.0f});

    input.move_mouse_to({50.0f, 50.0f}); // hover the content area
    shell.update();

    // A negative delta/notch (towards the user) increases the offset, i.e.
    // scrolls down: -2 * 40 = +80.
    input.scroll_wheel(-2);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 80.0f );

    // Far more than the remaining 120px (300 - 100 - 80) of room: clamped to 0.
    input.scroll_wheel(10);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollPanel:  clicking the vertical scroll bar's increment button moves the panel's own scroll position in sync" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::ScrollPanel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({200.0f, 100.0f});
    panel.set_content_size({100.0f, 300.0f});
    panel.set_scroll_position({0.0f, 100.0f});

    int emit_count = 0;
    panel.get_scroll_changed().connect([&](ml7::Vector2f) { ++emit_count; });

    // The (always shown, 16px thick) vertical scroll bar sits at local x
    // [184, 200), y [0, 100); its increment button is min(cross=16,
    // main*0.5=50) = 16px tall, spanning y [84, 100).
    input.move_mouse_to({190.0f, 90.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    // The scroll bar's step is kept in sync with get_wheel_step() (40 by
    // default), so clicking its increment button nudges the panel's own
    // scroll position by exactly that much: this is the ScrollPanel <->
    // ScrollBar wiring set up in ScrollPanel's constructor, not just the
    // ScrollBar's own click behavior (already covered by ScrollBar.cpp).
    TESTLABS_CHECK_EQ_ROUND( 3, panel.get_scroll_position().y, 140.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );
}
