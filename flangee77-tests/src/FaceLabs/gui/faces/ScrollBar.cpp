
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/ScrollBar.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollBar:  get_value()/set_value()/get_min()/get_max()/set_range()/get_step()/set_step()/get_changed() delegate to the inner slider" )
{
    fl7::gui::faces::ScrollBar scroll_bar(fl7::gui::faces::ScrollBar::Orientation::Vertical, 1.0f, 0.0f, 0.0f, 100.0f);

    int emit_count = 0;
    float last_value = -1.0f;
    scroll_bar.get_changed().connect([&](float value) { ++emit_count; last_value = value; });

    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_min(), 0.0f );
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_max(), 100.0f );

    scroll_bar.set_value(50.0f);
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_value(), 50.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ_ROUND( 3, last_value, 50.0f );

    scroll_bar.set_range(0.0f, 200.0f);
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_max(), 200.0f );

    scroll_bar.set_step(10.0f);
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_step(), 10.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollBar:  set_visible_fraction() clamps to [0, 1]" )
{
    fl7::gui::faces::ScrollBar scroll_bar(fl7::gui::faces::ScrollBar::Orientation::Vertical);
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_visible_fraction(), 1.0f );

    scroll_bar.set_visible_fraction(0.3f);
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_visible_fraction(), 0.3f );

    scroll_bar.set_visible_fraction(2.0f); // above 1: clamped
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_visible_fraction(), 1.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollBar:  get_show_buttons()/set_show_buttons()" )
{
    fl7::gui::faces::ScrollBar scroll_bar(fl7::gui::faces::ScrollBar::Orientation::Vertical);
    TESTLABS_CHECK( scroll_bar.get_show_buttons() );

    scroll_bar.set_show_buttons(false);
    TESTLABS_CHECK( !scroll_bar.get_show_buttons() );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollBar:  the decrement/increment buttons nudge the value by get_step()" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& scroll_bar = shell.add_face<fl7::gui::faces::ScrollBar>(fl7::gui::faces::ScrollBar::Orientation::Horizontal, 0.2f, 50.0f, 0.0f, 100.0f);
    scroll_bar.set_position({0.0f, 0.0f});
    scroll_bar.set_size({200.0f, 20.0f});

    // Each step button is min(cross, main * 0.5) = min(20, 100) = 20px wide:
    // the decrement button spans local x [0, 20), the increment button [180, 200).
    input.move_mouse_to({10.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_value(), 49.0f );

    input.move_mouse_to({190.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_value(), 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ScrollBar:  set_show_buttons(false) lets the slider fill the whole length" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& scroll_bar = shell.add_face<fl7::gui::faces::ScrollBar>(fl7::gui::faces::ScrollBar::Orientation::Horizontal, 0.1f, 50.0f, 0.0f, 100.0f);
    scroll_bar.set_position({0.0f, 0.0f});
    scroll_bar.set_size({200.0f, 20.0f});
    scroll_bar.set_show_buttons(false);

    // With the buttons hidden, local x=0 (which would otherwise be inside the
    // decrement button, see the test above) is now part of the slider's own
    // track, clicking there jumps straight to the minimum value. With the
    // buttons still occupying that space (the bug this guards against), the
    // hit-disabled button would simply have swallowed the click, leaving the
    // value unchanged at 50.
    input.move_mouse_to({0.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, scroll_bar.get_value(), 0.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}
