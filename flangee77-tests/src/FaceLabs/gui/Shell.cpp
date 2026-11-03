
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
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
