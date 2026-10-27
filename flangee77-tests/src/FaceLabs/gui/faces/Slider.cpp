
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Slider.h>

#include "../shared.h"



// Slider adds nothing of its own beyond a themed, fixed-square handle: these
// tests are really exercising TrackControl's shared value/step model and its
// click/drag/keyboard/wheel interaction, through the simplest concrete
// subclass of it.
//
// Expected values below are compared rounded to 3 decimals (see
// TESTLABS_CHECK_EQ_ROUND): they're the result of float divisions (e.g.
// 18.0f / 180.0f), which don't necessarily land on an exactly representable
// float, even though the math behind them is clean.



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  set_value() clamps to [min, max] and only emits get_changed() on an actual change" )
{
    fl7::gui::faces::Slider slider(fl7::gui::faces::Slider::Orientation::Horizontal, 0.0f, 0.0f, 100.0f);

    int emit_count = 0;
    float last_value = -1.0f;
    slider.get_changed().connect([&](float value) { ++emit_count; last_value = value; });

    slider.set_value(150.0f); // above max: clamped
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 100.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ_ROUND( 3, last_value, 100.0f );

    slider.set_value(100.0f); // already there: no-op
    TESTLABS_CHECK_EQ( emit_count, 1 );

    slider.set_value(-50.0f); // below min: clamped
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 0.0f );
    TESTLABS_CHECK_EQ( emit_count, 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  set_range() re-clamps the current value, emitting get_changed() only if it actually changed" )
{
    fl7::gui::faces::Slider slider(fl7::gui::faces::Slider::Orientation::Horizontal, 50.0f, 0.0f, 100.0f);

    int emit_count = 0;
    slider.get_changed().connect([&](float) { ++emit_count; });

    slider.set_range(0.0f, 200.0f); // still fits: value untouched
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 50.0f );
    TESTLABS_CHECK_EQ( emit_count, 0 );

    slider.set_range(0.0f, 10.0f); // no longer fits: re-clamped down to the new max
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 10.0f );
    TESTLABS_CHECK_EQ( emit_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  the default step is 1% of the initial range, and set_step() overrides it" )
{
    fl7::gui::faces::Slider slider(fl7::gui::faces::Slider::Orientation::Horizontal, 0.0f, 0.0f, 100.0f);
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_step(), 1.0f );

    slider.set_step(5.0f);
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_step(), 5.0f );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  clicking the track jumps the value to that position" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& slider = shell.add_face<fl7::gui::faces::Slider>(fl7::gui::faces::Slider::Orientation::Horizontal, 0.0f, 0.0f, 100.0f);
    slider.set_position({0.0f, 0.0f});
    slider.set_size({200.0f, 20.0f});

    // Handle is a fixed 20px square (the slider's own cross size); at value 0
    // it spans local x [0, 20), well clear of the click below.
    input.move_mouse_to({100.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    // Track range is 200 - 20 = 180px; a click centered at x=100 (handle
    // center at x - 10 = 90) maps to value 90 / 180 * 100 = 50.
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 50.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  clicking the handle itself does not jump, dragging it afterward moves the value by the drag delta" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& slider = shell.add_face<fl7::gui::faces::Slider>(fl7::gui::faces::Slider::Orientation::Horizontal, 50.0f, 0.0f, 100.0f);
    slider.set_position({0.0f, 0.0f});
    slider.set_size({200.0f, 20.0f});

    // At value 50 (of 180px track range), the handle spans local x [90, 110).
    input.move_mouse_to({100.0f, 10.0f});
    shell.update();

    // Pressing down without having moved the mouse first (delta == 0 this
    // frame) grabs the handle in place, without also triggering a drag.
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 50.0f );

    // Moving the mouse now (button still held) drags the already-grabbed
    // handle by the cursor's own delta: 18 / 180 * 100 = 10.
    input.move_mouse_to({118.0f, 10.0f});
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 60.0f );

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  Left/Down and Right/Up nudge the value by get_step(); Home/End jump to min/max" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& slider = shell.add_face<fl7::gui::faces::Slider>(fl7::gui::faces::Slider::Orientation::Horizontal, 0.0f, 0.0f, 100.0f);
    slider.set_position({0.0f, 0.0f});
    slider.set_size({200.0f, 20.0f});

    // Click on the handle (at value 0, spanning local x [0, 20)) to focus the
    // slider, without actually moving it.
    input.move_mouse_to({10.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == &slider );
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 0.0f );

    input.set_key_down(xl7::input::Key::Right, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 1.0f );
    input.set_key_down(xl7::input::Key::Right, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Up, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 2.0f );
    input.set_key_down(xl7::input::Key::Up, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Left, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 1.0f );
    input.set_key_down(xl7::input::Key::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 0.0f );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    input.set_key_down(xl7::input::Key::End, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 100.0f );
    input.set_key_down(xl7::input::Key::End, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 0.0f );
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Slider:  the mouse wheel nudges the value by get_step() per notch, clamped to [min, max]" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& slider = shell.add_face<fl7::gui::faces::Slider>(fl7::gui::faces::Slider::Orientation::Horizontal, 0.0f, 0.0f, 100.0f);
    slider.set_position({0.0f, 0.0f});
    slider.set_size({200.0f, 20.0f});

    input.move_mouse_to({100.0f, 10.0f});
    shell.update();

    input.scroll_wheel(-2); // away from the user: decreases the offset, i.e. increases the value
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 2.0f );

    input.scroll_wheel(3); // towards the user, and past 0: clamped to min
    shell.update();
    TESTLABS_CHECK_EQ_ROUND( 3, slider.get_value(), 0.0f );
}
