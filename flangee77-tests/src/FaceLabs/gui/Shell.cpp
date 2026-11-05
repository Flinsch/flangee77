
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/CheckBox.h>
#include <FaceLabs/gui/faces/Label.h>
#include <FaceLabs/gui/faces/ListBox.h>
#include <FaceLabs/gui/faces/Panel.h>
#include <FaceLabs/gui/faces/Slider.h>
#include <FaceLabs/gui/faces/TextField.h>

#include "./shared.h"



TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  clicking a different top-level face moves focus there, blurring the previous one; clicking empty space blurs it with nothing taking its place" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button_a = shell.add_face<fl7::gui::faces::Button>(U"A");
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = shell.add_face<fl7::gui::faces::Button>(U"B");
    button_b.set_position({100.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});

    TESTLABS_CHECK( shell.get_focused_face() == nullptr );

    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a) );
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({125.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_b) );
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({500.0f, 500.0f}); // empty space: hits nothing
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == nullptr );
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  of two overlapping top-level faces, the one added later (topmost in Z-order) wins hover/hit-testing in their overlap" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& back = shell.add_face<fl7::gui::faces::Panel>();
    back.set_position({0.0f, 0.0f});
    back.set_size({100.0f, 100.0f});

    auto& front = shell.add_face<fl7::gui::faces::Panel>(); // added after back: topmost
    front.set_position({50.0f, 50.0f});
    front.set_size({100.0f, 100.0f}); // overlaps back's bottom-right quadrant, [50, 100) x [50, 100)

    input.move_mouse_to({25.0f, 25.0f}); // back only
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&back) );

    input.move_mouse_to({125.0f, 125.0f}); // front only
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&front) );

    input.move_mouse_to({75.0f, 75.0f}); // the overlap itself
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&front) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  clicking a background top-level face brings it to the front, so it now wins the overlap too" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& back = shell.add_face<fl7::gui::faces::Panel>();
    back.set_position({0.0f, 0.0f});
    back.set_size({100.0f, 100.0f});

    auto& front = shell.add_face<fl7::gui::faces::Panel>(); // added after back: topmost for now
    front.set_position({50.0f, 50.0f});
    front.set_size({100.0f, 100.0f}); // overlaps back's bottom-right quadrant, [50, 100) x [50, 100)

    input.move_mouse_to({75.0f, 75.0f}); // the overlap, before any click
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&front) );

    input.move_mouse_to({25.0f, 25.0f}); // back's own exclusive area: not currently topmost, but still hit
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({75.0f, 75.0f}); // the overlap again: back is now in front instead
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&back) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  clicking empty space (no top-level face hit) leaves the Z-order untouched" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& back = shell.add_face<fl7::gui::faces::Panel>();
    back.set_position({0.0f, 0.0f});
    back.set_size({100.0f, 100.0f});

    auto& front = shell.add_face<fl7::gui::faces::Panel>();
    front.set_position({50.0f, 50.0f});
    front.set_size({100.0f, 100.0f});

    input.move_mouse_to({500.0f, 500.0f}); // hits neither
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.move_mouse_to({75.0f, 75.0f}); // the overlap: still front, unchanged
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&front) );
}



// #############################################################################
// Popups (open_popup()/close_popup())
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  open_popup() raises the given popup to the front of the Z-order, and keeps it there on every repeated call" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& owner = shell.add_face<fl7::gui::faces::Panel>();
    owner.set_position({0.0f, 0.0f});
    owner.set_size({100.0f, 100.0f});

    auto& popup = shell.add_face<fl7::gui::faces::Panel>();
    popup.set_position({0.0f, 0.0f});
    popup.set_size({100.0f, 100.0f}); // fully overlapping owner

    shell.open_popup(&popup, &owner);

    input.move_mouse_to({50.0f, 50.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&popup) );

    // A third, unrelated top-level face, added afterwards (frontmost by
    // construction, same as any newly added one), ends up ahead of the popup...
    auto& other = shell.add_face<fl7::gui::faces::Panel>();
    other.set_position({0.0f, 0.0f});
    other.set_size({100.0f, 100.0f});

    input.move_mouse_to({50.1f, 50.0f}); // re-trigger hit-testing
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&other) );

    // ...but re-opening the popup raises it again, ahead of that too.
    shell.open_popup(&popup, &owner);
    input.move_mouse_to({50.2f, 50.0f});
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&popup) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  open_popup() auto-closes the popup once focus moves to a face outside both it and its owner" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& owner = shell.add_face<fl7::gui::faces::Button>(U"Owner");
    owner.set_position({0.0f, 0.0f});
    owner.set_size({50.0f, 20.0f});

    auto& popup = shell.add_face<fl7::gui::faces::Panel>();
    popup.set_position({0.0f, 30.0f});
    popup.set_size({50.0f, 50.0f});

    auto& other = shell.add_face<fl7::gui::faces::Button>(U"Other");
    other.set_position({200.0f, 0.0f});
    other.set_size({50.0f, 20.0f});

    shell.open_popup(&popup, &owner);
    TESTLABS_CHECK( popup.is_visible() );

    input.move_mouse_to({210.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( !popup.is_visible() ); // closed the moment focus actually moved to `other`

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  open_popup() does not auto-close the popup when focus lands back on its owner, or within its own content" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& owner = shell.add_face<fl7::gui::faces::Button>(U"Owner");
    owner.set_position({0.0f, 0.0f});
    owner.set_size({50.0f, 20.0f});

    auto& popup = shell.add_face<fl7::gui::faces::Button>(U"Popup content"); // focusable, so clicking it really moves focus
    popup.set_position({0.0f, 30.0f});
    popup.set_size({50.0f, 20.0f});

    shell.open_popup(&popup, &owner);

    // Clicking into the popup's own content keeps it open.
    input.move_mouse_to({25.0f, 40.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( popup.is_visible() );
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    // Clicking back onto the owner keeps it open too (its own click handling,
    // e.g. toggling it closed again, is a separate concern entirely).
    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK( popup.is_visible() );
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  bringing a popup's owner to the front also raises the popup, so it never visually slips behind it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& window = shell.add_face<fl7::gui::faces::Panel>();
    window.set_position({0.0f, 0.0f});
    window.set_size({100.0f, 100.0f});

    auto& owner = window.add_child<fl7::gui::faces::Button>(U"Owner");
    owner.set_position({10.0f, 10.0f});
    owner.set_size({30.0f, 20.0f}); // top area, (10, 10)-(40, 30), not covered by popup below

    auto& popup = shell.add_face<fl7::gui::faces::Panel>(); // added after window: topmost for now
    popup.set_position({0.0f, 50.0f});
    popup.set_size({100.0f, 50.0f}); // window's bottom half, (0, 50)-(100, 100)

    shell.open_popup(&popup, &owner);

    input.move_mouse_to({50.0f, 75.0f}); // within popup's area (and window's, underneath it)
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&popup) );

    // Clicking `owner` (within `window`) raises `window` to the front. Without
    // the fix in _bring_to_front(), this would leave `popup` stranded behind
    // `window` until something eventually closes it; with the fix, `popup`
    // gets raised right along with it, staying on top throughout.
    input.move_mouse_to({25.0f, 20.0f}); // owner's own rect, not covered by popup
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    input.move_mouse_to({50.0f, 75.0f}); // back to popup's area
    shell.update();
    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&popup) ); // still on top, not `window`
    TESTLABS_CHECK( popup.is_visible() ); // never closed by this -- it's a click on its own owner

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}



TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  removing a child face that's currently hovered/pressed/focused clears that state instead of leaving it dangling" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({200.0f, 100.0f});

    auto& button = panel.add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({10.0f, 10.0f});
    button.set_size({80.0f, 24.0f});

    input.move_mouse_to({50.0f, 20.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();

    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&button) );
    TESTLABS_CHECK( shell.get_pressed_face() == static_cast<fl7::gui::Face*>(&button) );
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button) );

    // Removed while still mouse-captured (not yet released): exactly the
    // situation Shell::_handle_face_removed() exists for.
    panel.remove_child(button);

    TESTLABS_CHECK( shell.get_hovered_face() == nullptr );
    TESTLABS_CHECK( shell.get_pressed_face() == nullptr );
    TESTLABS_CHECK( shell.get_focused_face() == nullptr );

    // The still-pending release must not try to touch the now-gone face either.
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}



// #############################################################################
// Tab/Shift+Tab focus navigation
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Tab cycles focus forward through focusable controls in tree order, skipping non-focusable faces, wrapping around at the end" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>(); // not itself a Tab stop
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    panel.add_child<fl7::gui::faces::Label>(U"Caption"); // not a Tab stop either

    auto& button_a = panel.add_child<fl7::gui::faces::Button>(U"A");
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = panel.add_child<fl7::gui::faces::Button>(U"B");
    button_b.set_position({100.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});
    auto& button_c = panel.add_child<fl7::gui::faces::Button>(U"C");
    button_c.set_position({200.0f, 0.0f});
    button_c.set_size({50.0f, 20.0f});

    // Click button_a first, to establish it (and this top-level panel) as the current focus.
    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a) );

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_b) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_c) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    // Wraps back around to button_a.
    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Shift+Tab cycles focus backward instead, wrapping around at the start" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    auto& button_a = panel.add_child<fl7::gui::faces::Button>(U"A");
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = panel.add_child<fl7::gui::faces::Button>(U"B");
    button_b.set_position({100.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});
    auto& button_c = panel.add_child<fl7::gui::faces::Button>(U"C");
    button_c.set_position({200.0f, 0.0f});
    button_c.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click button_a first
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftShift, true);

    input.set_key_down(xl7::input::Key::Tab, true); // wraps straight to button_c, backward from button_a
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_c) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_b) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftShift, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  an explicit tab_order takes priority over tree order; faces without one keep falling back to it, trailing behind every explicit one" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    auto& button_a = panel.add_child<fl7::gui::faces::Button>(U"A"); // added first, but given a higher explicit order
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = panel.add_child<fl7::gui::faces::Button>(U"B"); // added second, no explicit order at all
    button_b.set_position({100.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});
    auto& button_c = panel.add_child<fl7::gui::faces::Button>(U"C"); // added last, but given the lowest explicit order
    button_c.set_position({200.0f, 0.0f});
    button_c.set_size({50.0f, 20.0f});

    button_a.set_tab_order(1);
    button_c.set_tab_order(0);
    // button_b keeps no explicit tab_order: expected order is button_c (0), button_a (1), then button_b trailing behind.

    input.move_mouse_to({225.0f, 10.0f}); // click button_c first
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_c) );

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_b) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Tab, true); // wraps back around to button_c
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_c) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Tab stays scoped to the currently focused face's own top-level face, never crossing into a different one" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& window_a = shell.add_face<fl7::gui::faces::Panel>();
    window_a.set_position({0.0f, 0.0f});
    window_a.set_size({300.0f, 100.0f});
    auto& button_a1 = window_a.add_child<fl7::gui::faces::Button>(U"A1");
    button_a1.set_position({0.0f, 0.0f});
    button_a1.set_size({50.0f, 20.0f});
    auto& button_a2 = window_a.add_child<fl7::gui::faces::Button>(U"A2");
    button_a2.set_position({100.0f, 0.0f});
    button_a2.set_size({50.0f, 20.0f});

    auto& window_b = shell.add_face<fl7::gui::faces::Panel>();
    window_b.set_position({0.0f, 200.0f});
    window_b.set_size({300.0f, 100.0f});
    auto& button_b1 = window_b.add_child<fl7::gui::faces::Button>(U"B1");
    button_b1.set_position({0.0f, 0.0f});
    button_b1.set_size({50.0f, 20.0f});

    // Focus the last control of window_a.
    input.move_mouse_to({125.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a2) );

    // Tab wraps back to button_a1, never reaching window_b's button_b1.
    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a1) );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Tab does nothing while nothing is currently focused yet (no current top-level face to scope to)" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});
    auto& button = panel.add_child<fl7::gui::faces::Button>(U"A");
    button.set_position({0.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    TESTLABS_CHECK( shell.get_focused_face() == nullptr );

    input.set_key_down(xl7::input::Key::Tab, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == nullptr );
    input.set_key_down(xl7::input::Key::Tab, false);
    shell.update();
}



// #############################################################################
// Arrow-key/gamepad spatial focus navigation (Shell::focus_toward())
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  arrow keys move focus spatially around a 2x2 grid of controls" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 300.0f});

    auto& top_left = panel.add_child<fl7::gui::faces::Button>(U"TL");
    top_left.set_position({0.0f, 0.0f});
    top_left.set_size({50.0f, 20.0f});
    auto& top_right = panel.add_child<fl7::gui::faces::Button>(U"TR");
    top_right.set_position({200.0f, 0.0f});
    top_right.set_size({50.0f, 20.0f});
    auto& bottom_left = panel.add_child<fl7::gui::faces::Button>(U"BL");
    bottom_left.set_position({0.0f, 200.0f});
    bottom_left.set_size({50.0f, 20.0f});
    auto& bottom_right = panel.add_child<fl7::gui::faces::Button>(U"BR");
    bottom_right.set_position({200.0f, 200.0f});
    bottom_right.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click top_left
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&top_left) );

    shell.focus_toward(fl7::gui::ArrowDirection::Right);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&top_right) );

    shell.focus_toward(fl7::gui::ArrowDirection::Down);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&bottom_right) );

    shell.focus_toward(fl7::gui::ArrowDirection::Left);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&bottom_left) );

    shell.focus_toward(fl7::gui::ArrowDirection::Up);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&top_left) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  with nothing focused yet, an arrow key enters the top-level face at the outermost control in that direction" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    auto& left = panel.add_child<fl7::gui::faces::Button>(U"Left");
    left.set_position({0.0f, 0.0f});
    left.set_size({50.0f, 20.0f});
    auto& right = panel.add_child<fl7::gui::faces::Button>(U"Right");
    right.set_position({200.0f, 0.0f});
    right.set_size({50.0f, 20.0f});

    TESTLABS_CHECK( shell.get_focused_face() == nullptr );

    shell.focus_toward(fl7::gui::ArrowDirection::Right); // enters at the outermost control towards the right
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&right) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a focused TextField keeps the horizontal arrows for its caret (soft); Alt+arrow still moves focus away" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    auto& text_field = panel.add_child<fl7::gui::faces::TextField>();
    text_field.set_position({0.0f, 0.0f});
    text_field.set_size({50.0f, 20.0f});
    auto& button = panel.add_child<fl7::gui::faces::Button>(U"B");
    button.set_position({200.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click the TextField
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&text_field) );

    shell.focus_toward(fl7::gui::ArrowDirection::Right, false); // plain: the caret consumes it, focus stays
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&text_field) );

    shell.focus_toward(fl7::gui::ArrowDirection::Right, true); // forced: a soft consumer yields
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a focused TrackControl keeps every arrow for itself (hard), even when forced" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    auto& slider = panel.add_child<fl7::gui::faces::Slider>();
    slider.set_position({0.0f, 0.0f});
    slider.set_size({50.0f, 20.0f});
    auto& button = panel.add_child<fl7::gui::faces::Button>(U"B");
    button.set_position({200.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click the slider
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&slider) );

    shell.focus_toward(fl7::gui::ArrowDirection::Right, false);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&slider) );

    shell.focus_toward(fl7::gui::ArrowDirection::Right, true); // even forced, a hard consumer never yields
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&slider) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a focused ListBox keeps Down while its selection still has somewhere to go, yielding to focus navigation once at the last item" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 300.0f});

    auto& list_box = panel.add_child<fl7::gui::faces::ListBox>();
    list_box.set_position({0.0f, 0.0f});
    list_box.set_size({200.0f, 60.0f}); // 2 rows' worth of visible height, default row height 24px
    for (int i = 0; i < 2; ++i)
        list_box.add_item<fl7::gui::faces::Label>(U"Item");

    auto& button = panel.add_child<fl7::gui::faces::Button>(U"B");
    button.set_position({0.0f, 200.0f});
    button.set_size({50.0f, 20.0f});

    input.move_mouse_to({50.0f, 5.0f}); // click row 0, focusing the list
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 0 );

    input.set_key_down(xl7::input::Key::Down, true); // moves the selection, not the focus: still room to go (row 1)
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 1 );
    TESTLABS_CHECK( shell.get_focused_face() != nullptr && shell.get_focused_face() != static_cast<fl7::gui::Face*>(&button) ); // focus stayed within the list
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    input.set_key_down(xl7::input::Key::Down, true); // already at the last item: yields to spatial navigation instead
    shell.update();
    TESTLABS_CHECK_EQ( list_box.get_selected_index(), 1 ); // unchanged: _on_key_down() clamps it to the last item anyway
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button) );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a top-level face with arrow navigation disabled ignores focus_toward() entirely" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});
    panel.set_arrow_navigation_enabled(false);

    auto& button_a = panel.add_child<fl7::gui::faces::Button>(U"A");
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = panel.add_child<fl7::gui::faces::Button>(U"B");
    button_b.set_position({200.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click button_a
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    shell.focus_toward(fl7::gui::ArrowDirection::Right);
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button_a) ); // unchanged: navigation is disabled for this top-level face
}



// #############################################################################
// Enter/Space/gamepad "confirm" activation (Shell::click_focus())
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Enter activates a focused Button exactly like a click" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"Caption");
    button.set_position({0.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( click_count, 1 ); // the actual click itself, nothing to do with Enter yet

    input.set_key_down(xl7::input::Key::Enter, true);
    shell.update();
    TESTLABS_CHECK_EQ( click_count, 2 );
    input.set_key_down(xl7::input::Key::Enter, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  Space activates a focused CheckBox exactly like a click, toggling it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& check_box = shell.add_face<fl7::gui::faces::CheckBox>();
    check_box.set_position({0.0f, 0.0f});
    check_box.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click to focus it, without toggling via Space yet
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( check_box.is_checked() ); // the click itself already toggled it on

    input.set_key_down(xl7::input::Key::Space, true);
    shell.update();
    TESTLABS_CHECK( !check_box.is_checked() );
    input.set_key_down(xl7::input::Key::Space, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a focused face that isn't clickable (e.g. a Panel) ignores Enter/Space; can_click_focus() reports false" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({0.0f, 0.0f});
    panel.set_size({300.0f, 100.0f});

    input.move_mouse_to({25.0f, 10.0f}); // click empty space inside the panel: focuses the panel itself
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&panel) );
    TESTLABS_CHECK( !shell.can_click_focus() );

    input.set_key_down(xl7::input::Key::Enter, true); // a no-op: nothing to activate
    shell.update();
    input.set_key_down(xl7::input::Key::Enter, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  click_focus() can be driven directly (e.g. by a gamepad's \"confirm\" button), without a literal Enter/Space keypress" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"Caption");
    button.set_position({0.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK_EQ( click_count, 1 );

    TESTLABS_CHECK( shell.can_click_focus() );
    TESTLABS_CHECK( shell.click_focus() );
    TESTLABS_CHECK_EQ( click_count, 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Shell:  a focused but since-disabled Button can no longer be activated via click_focus(), even though it's still focused" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"Caption");
    button.set_position({0.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button) );

    button.set_enabled(false); // disabling it doesn't itself clear the focus

    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&button) );
    TESTLABS_CHECK( !shell.can_click_focus() );
    TESTLABS_CHECK( !shell.click_focus() );
}
