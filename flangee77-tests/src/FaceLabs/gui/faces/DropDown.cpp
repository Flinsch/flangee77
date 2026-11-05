
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/DropDown.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  add_item()/get_item_count(), and get_display_text() defaults to empty with nothing selected" )
{
    fl7::gui::faces::DropDown drop_down;
    TESTLABS_CHECK_EQ( drop_down.get_item_count(), 0 );
    TESTLABS_CHECK( drop_down.get_display_text().empty() );

    drop_down.add_item(U"A");
    drop_down.add_item(U"B");
    TESTLABS_CHECK_EQ( drop_down.get_item_count(), 2 );
    TESTLABS_CHECK( drop_down.get_display_text().empty() );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  set_selected_index() clamps/ignores out-of-range and only emits get_selection_changed() on an actual change" )
{
    fl7::gui::faces::DropDown drop_down;
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");

    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), -1 );

    int emit_count = 0;
    int last_index = -2;
    drop_down.get_selection_changed().connect([&](int index) { ++emit_count; last_index = index; });

    drop_down.set_selected_index(5); // out of range (besides -1): ignored
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), -1 );
    TESTLABS_CHECK_EQ( emit_count, 0 );

    drop_down.set_selected_index(1);
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), 1 );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ( last_index, 1 );
    TESTLABS_CHECK_EQ( drop_down.get_display_text(), U"B" );

    drop_down.set_selected_index(1); // already selected: no-op
    TESTLABS_CHECK_EQ( emit_count, 1 );

    drop_down.set_selected_index(-1);
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), -1 );
    TESTLABS_CHECK_EQ( emit_count, 2 );
    TESTLABS_CHECK( drop_down.get_display_text().empty() );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  clicking it opens a popup list right below it; clicking an item there selects it, closes the popup again, and returns focus here" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");
    drop_down.add_item(U"C");

    // Nothing is there yet at the popup's future position (below the drop-down),
    // confirming the popup really is only created once first opened, not upfront.
    input.move_mouse_to({50.0f, 30.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );

    // Click the (closed) drop-down itself: opens the popup.
    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down );

    // The popup's row 1 ("B") now sits at local y [24, 48) below the drop-down,
    // i.e. absolute y [44, 68), i.e. screen y 50 hits it.
    input.move_mouse_to({50.0f, 50.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() != nullptr );

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), 1 );
    TESTLABS_CHECK_EQ( drop_down.get_display_text(), U"B" );

    // The popup is closed again (nothing there anymore), and focus is back here.
    input.move_mouse_to({50.0f, 50.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  Escape closes an open popup without changing the selection" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({50.0f, 30.0f}); // row 0, below the drop-down
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() != nullptr );

    input.set_key_down(xl7::input::Key::Escape, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Escape, false);
    shell.update();

    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), -1 ); // unchanged

    input.move_mouse_to({50.0f, 30.5f}); // re-trigger hit-testing at the same spot
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr ); // popup gone
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  Up/Down move the selection while the popup is open, without moving focus off of it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");
    drop_down.add_item(U"C");

    // Down before the popup is ever opened does nothing (closed drop-downs don't
    // scrub through values on their own, see DropDown::_on_key_down()).
    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down );

    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), 0 );
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down ); // stayed here, never moved to the popup
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), 1 );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Up, true);
    shell.update();
    TESTLABS_CHECK_EQ( drop_down.get_selected_index(), 0 );
    input.set_key_down(xl7::input::Key::Up, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  clicking away from an open popup closes it and clears focus, same as clicking empty space normally does" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({500.0f, 500.0f}); // empty space, nowhere near the drop-down or its popup
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK( shell.get_focused_face() == nullptr );

    input.move_mouse_to({50.0f, 30.0f}); // where the popup's row 0 would be
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == nullptr ); // popup gone
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  clicking the trailing arrow button specifically also opens the popup" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");
    drop_down.add_item(U"B");

    // The arrow button is square, sized to the drop-down's own height (20px),
    // so it spans local/absolute x [80, 100); x=90 hits it, not the text part.
    input.move_mouse_to({90.0f, 10.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() != nullptr );
    TESTLABS_CHECK( shell.get_hovered_face() != &drop_down ); // the button itself, not the drop-down as a whole

    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    // The popup opened all the same (row 0, "A", at absolute y [20, 44)).
    input.move_mouse_to({50.0f, 30.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() != nullptr );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  DropDown:  the arrow button is its own Tab stop, separate from the drop-down itself" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& drop_down = shell.add_face<fl7::gui::faces::DropDown>();
    drop_down.set_position({0.0f, 0.0f});
    drop_down.set_size({100.0f, 20.0f});
    drop_down.add_item(U"A");

    input.move_mouse_to({50.0f, 10.0f}); // the text part, not the arrow button
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down );

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() != &drop_down ); // moved on to the arrow button
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == &drop_down ); // only two Tab stops: wrapped back around
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();
}
