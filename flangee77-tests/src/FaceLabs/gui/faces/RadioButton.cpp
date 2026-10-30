
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/RadioGroup.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/RadioButton.h>

#include "../shared.h"



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  set_checked(true) notifies the group, unchecking whichever other member was previously checked" )
{
    fl7::gui::RadioGroup group;
    fl7::gui::faces::RadioButton button_a(&group);
    fl7::gui::faces::RadioButton button_b(&group);

    int emit_count_a = 0;
    int emit_count_b = 0;
    button_a.get_changed().connect([&](bool) { ++emit_count_a; });
    button_b.get_changed().connect([&](bool) { ++emit_count_b; });

    button_a.set_checked(true);
    TESTLABS_CHECK( button_a.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button_a );
    TESTLABS_CHECK_EQ( emit_count_a, 1 );
    TESTLABS_CHECK_EQ( emit_count_b, 0 );

    button_b.set_checked(true);
    TESTLABS_CHECK( !button_a.is_checked() ); // unchecked by the group, not directly
    TESTLABS_CHECK( button_b.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button_b );
    TESTLABS_CHECK_EQ( emit_count_a, 2 ); // the forced uncheck still emits get_changed()
    TESTLABS_CHECK_EQ( emit_count_b, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  explicitly unchecking a radio button is allowed and forgets it from the group" )
{
    fl7::gui::RadioGroup group;
    fl7::gui::faces::RadioButton button(&group, true); // starts checked
    TESTLABS_CHECK( button.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button );

    button.set_checked(false);
    TESTLABS_CHECK( !button.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == nullptr );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  destroying the checked member forgets it from the group, instead of leaving a dangling pointer" )
{
    fl7::gui::RadioGroup group;
    TESTLABS_CHECK( group.get_checked_button() == nullptr );

    {
        fl7::gui::faces::RadioButton button(&group, true);
        TESTLABS_CHECK( group.get_checked_button() == &button );
    }

    TESTLABS_CHECK( group.get_checked_button() == nullptr );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  clicking an unchecked radio button checks it and unchecks whichever sibling in its group was checked" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::RadioGroup group;
    auto& button_a = shell.add_face<fl7::gui::faces::RadioButton>(&group, true);
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({100.0f, 20.0f});
    auto& button_b = shell.add_face<fl7::gui::faces::RadioButton>(&group, false);
    button_b.set_position({0.0f, 30.0f});
    button_b.set_size({100.0f, 20.0f});

    TESTLABS_CHECK( button_a.is_checked() );
    TESTLABS_CHECK( !button_b.is_checked() );

    input.move_mouse_to({50.0f, 40.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK( !button_a.is_checked() );
    TESTLABS_CHECK( button_b.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button_b );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  clicking an already-checked radio button does nothing (never toggles off, unlike CheckBox)" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::RadioGroup group;
    auto& button = shell.add_face<fl7::gui::faces::RadioButton>(&group, true);
    button.set_position({0.0f, 0.0f});
    button.set_size({100.0f, 20.0f});

    int emit_count = 0;
    button.get_changed().connect([&](bool) { ++emit_count; });

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    TESTLABS_CHECK( button.is_checked() );
    TESTLABS_CHECK_EQ( emit_count, 0 );
}
