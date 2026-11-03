
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/HasBackground.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Panel.h>

#include "../shared.h"

#include <type_traits>



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Panel:  is directly a Container (unlike Window's Compound-gated content area): add_child()/remove_child() are public on the Panel itself" )
{
    fl7::gui::faces::Panel panel;

    auto& button = panel.add_child<fl7::gui::faces::Button>(U"Caption");
    TESTLABS_CHECK( button.get_parent() == static_cast<fl7::gui::Face*>(&panel) );

    panel.remove_child(button);
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Panel:  is recognized as a HasBackground; a bare Container is not" )
{
    // A structural fact, true (and provably so) for every instance, not just the
    // one constructed below: a plain Container's exact dynamic type is always
    // Container itself here, so GCC flags a dynamic_cast to the unrelated
    // HasBackground as "can never succeed" (-Werror); std::is_base_of_v
    // expresses the same thing without tripping over that.
    static_assert( std::is_base_of_v<fl7::gui::HasBackground, fl7::gui::faces::Panel> );
    static_assert( !std::is_base_of_v<fl7::gui::HasBackground, fl7::gui::Container> );

    fl7::gui::faces::Panel panel;
    TESTLABS_CHECK( dynamic_cast<fl7::gui::HasBackground*>(&panel) != nullptr );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Panel:  a button added directly to the Panel is hit-tested/clicked right through it, unoffset" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    panel.set_position({10.0f, 10.0f});
    panel.set_size({200.0f, 150.0f});

    auto& button = panel.add_child<fl7::gui::faces::Button>(U"Caption");
    button.set_position({20.0f, 20.0f});
    button.set_size({80.0f, 24.0f});

    int click_count = 0;
    button.get_clicked().connect([&]() { ++click_count; });

    // Button spans the panel's absolute (10+20, 10+20) to (10+100, 10+44).
    input.move_mouse_to({60.0f, 30.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK_EQ( click_count, 1 );
}
