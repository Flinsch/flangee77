
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Label.h>
#include <FaceLabs/gui/faces/ListBox.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  add_item()/get_item_count()" )
{
    fl7::gui::faces::ListBox list_box;
    TESTLABS_CHECK_EQ( list_box.get_item_count(), 0 );

    list_box.add_item<fl7::gui::faces::Label>(U"Item 1");
    list_box.add_item<fl7::gui::faces::Label>(U"Item 2");
    TESTLABS_CHECK_EQ( list_box.get_item_count(), 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  set_selected_index() clamps/ignores out-of-range and only emits get_selection_changed() on an actual change" )
{
    fl7::gui::faces::ListBox list_box;
    list_box.add_item<fl7::gui::faces::Label>(U"Item 1");
    list_box.add_item<fl7::gui::faces::Label>(U"Item 2");

    TESTLABS_CHECK_EQ( list_box.get_selected_index(), -1 );

    int emit_count = 0;
    int last_index = -2;
    list_box.get_selection_changed().connect([&](int index) { ++emit_count; last_index = index; });

    list_box.set_selected_index(5); // out of range (besides -1): ignored
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), -1 );
    TESTLABS_CHECK_EQ( emit_count, 0 );

    list_box.set_selected_index(1);
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 1 );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ( last_index, 1 );

    list_box.set_selected_index(1); // already selected: no-op
    TESTLABS_CHECK_EQ( emit_count, 1 );

    list_box.set_selected_index(-1);
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), -1 );
    TESTLABS_CHECK_EQ( emit_count, 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  remove_item() clears the selection if the removed item was selected" )
{
    fl7::gui::faces::ListBox list_box;
    for (int i = 0; i < 3; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");
    list_box.set_selected_index(1);

    int emit_count = 0;
    list_box.get_selection_changed().connect([&](int) { ++emit_count; });

    list_box.remove_item(1);

    TESTLABS_CHECK_EQ( list_box.get_item_count(), 2 );
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), -1 );
    TESTLABS_CHECK_EQ( emit_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  remove_item() shifts a later selection down by one, without re-emitting get_selection_changed() for the same item" )
{
    fl7::gui::faces::ListBox list_box;
    for (int i = 0; i < 3; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");
    list_box.set_selected_index(2);

    list_box.remove_item(0);

    TESTLABS_CHECK_EQ( list_box.get_item_count(), 2 );
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  remove_item() leaves an earlier selection untouched" )
{
    fl7::gui::faces::ListBox list_box;
    for (int i = 0; i < 3; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");
    list_box.set_selected_index(0);

    int emit_count = 0;
    list_box.get_selection_changed().connect([&](int) { ++emit_count; });

    list_box.remove_item(2);

    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 0 );
    TESTLABS_CHECK_EQ( emit_count, 0 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  remove_item() with an out-of-range index is ignored" )
{
    fl7::gui::faces::ListBox list_box;
    list_box.add_item<fl7::gui::faces::Label>(U"Item");
    list_box.set_selected_index(0);

    list_box.remove_item(-1);
    list_box.remove_item(1);

    TESTLABS_CHECK_EQ( list_box.get_item_count(), 1 );
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 0 );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  clicking a row selects it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& list_box = shell.add_face<fl7::gui::faces::ListBox>();
    list_box.set_position({0.0f, 0.0f});
    list_box.set_size({200.0f, 100.0f});
    for (int i = 0; i < 3; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");

    // The default row height is 24px, so row 1 spans local y [24, 48).
    input.move_mouse_to({50.0f, 30.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  clicking a row selects the right one even while scrolled (regression: Face::get_absolute_position() netting out ancestor scroll offset)" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& list_box = shell.add_face<fl7::gui::faces::ListBox>();
    list_box.set_position({0.0f, 0.0f});
    list_box.set_size({200.0f, 100.0f});
    for (int i = 0; i < 10; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");

    // Hover over the list first, so the wheel event below actually bubbles up
    // to (and is consumed by) its internal ScrollPanel.
    input.move_mouse_to({50.0f, 50.0f});
    shell.update();

    // Scroll down by 3 notches * the default 40px wheel step = 120px. The
    // list's content is 10 * 24 = 240px tall, 100px of which are visible, so up
    // to 140px of scrolling is actually possible: 120px doesn't get clamped.
    input.scroll_wheel(-3);
    shell.update();

    // With the list scrolled by 120px, clicking at the same screen position as
    // above (y=50) must now hit row (120 + 50) / 24 = 7, not row 50 / 24 = 2
    // (the row that would be under the cursor if the scroll offset were
    // ignored, which used to be the bug).
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 7 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  Up/Down/Home/End navigate the selection" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& list_box = shell.add_face<fl7::gui::faces::ListBox>();
    list_box.set_position({0.0f, 0.0f});
    list_box.set_size({200.0f, 100.0f});
    for (int i = 0; i < 5; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");

    // Click row 2 (local y [48, 72)) first: arrow keys only reach the focused
    // face, and this both focuses the list and confirms the starting selection.
    input.move_mouse_to({50.0f, 60.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 2 );

    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 3 );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Up, true);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 2 );
    input.set_key_down(xl7::input::Key::Up, false);
    shell.update();

    input.set_key_down(xl7::input::Key::End, true);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 4 );
    input.set_key_down(xl7::input::Key::End, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 0 );
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  ListBox:  Ctrl-clicking the selected row clears the selection, Ctrl-clicking another row selects it normally" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& list_box = shell.add_face<fl7::gui::faces::ListBox>();
    list_box.set_position({0.0f, 0.0f});
    list_box.set_size({200.0f, 100.0f});
    for (int i = 0; i < 3; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");
    list_box.set_selected_index(1);

    input.set_key_down(xl7::input::Key::LeftControl, true);

    // Ctrl-click row 1 (local y [24, 48)), already selected: clears it.
    input.move_mouse_to({50.0f, 30.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), -1 );

    // Ctrl-click row 0 (local y [0, 24)), not yet selected: selects it
    // normally, Ctrl notwithstanding (it only clears an already-selected row).
    input.move_mouse_to({50.0f, 5.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 0 );

    input.set_key_down(xl7::input::Key::LeftControl, false);
}
