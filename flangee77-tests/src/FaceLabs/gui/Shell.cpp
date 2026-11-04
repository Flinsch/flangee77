
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Label.h>
#include <FaceLabs/gui/faces/Panel.h>

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
