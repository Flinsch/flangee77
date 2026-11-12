
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/RadioGroup.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Label.h>
#include <FaceLabs/gui/faces/RadioButton.h>

#include "../../shared.h"
#include "../shared.h"



namespace {

    using f77_tests::fonts::get_test_font;

} // namespace



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
// Preferred/minimum size (box glyph + gap + caption's own text-based size)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  without an explicit override, get_preferred_size()/get_minimum_size() stay {0, 0} regardless of font; get_effective_preferred_size()/get_effective_minimum_size() need the caption to have an effective font to calculate anything, else also {0, 0}" )
{
    fl7::gui::RadioGroup group;
    fl7::gui::faces::RadioButton button(&group, false, U"Caption");
    TESTLABS_CHECK_EQ_FLT( button.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_minimum_size().x, 0.0f );

    TESTLABS_CHECK_EQ_FLT( button.get_effective_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  RadioButton:  set_style_override() with a real font: get_effective_preferred_size()/get_effective_minimum_size() reflect the box glyph (square, side = caption height) plus a fixed gap plus the caption's own natural (unwrapped) size, growing with more text; get_preferred_size()/get_minimum_size() stay {0, 0} regardless (no explicit override set); the caption (a child Label) inherits the override from its RadioButton parent, same as any other nested style override" )
{
    constexpr float caption_gap = 6.0f; // RadioButton::CAPTION_GAP; mirrored here, it's private.

    fl7::gui::Style style;
    style.font = &get_test_font();

    // An unparented reference Label under the same override, to independently
    // compute what the caption's own natural size should be.
    fl7::gui::faces::Label reference_label(U"Caption");
    reference_label.set_style_override(style);
    const ml7::Vector2f caption_size = reference_label.get_effective_preferred_size();
    TESTLABS_CHECK( caption_size.x > 0.0f );
    TESTLABS_CHECK( caption_size.y > 0.0f );

    fl7::gui::RadioGroup group;
    fl7::gui::faces::RadioButton button(&group, false, U"Caption");
    button.set_style_override(style); // the caption Label child inherits this, same as any nested override.

    const ml7::Vector2f preferred = button.get_effective_preferred_size();
    TESTLABS_CHECK_EQ_FLT( preferred.x, caption_size.y + caption_gap + caption_size.x );
    TESTLABS_CHECK_EQ_FLT( preferred.y, caption_size.y );
    // The box glyph's own minimum doesn't constrain this further: same formula either way.
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().x, preferred.x );
    TESTLABS_CHECK_EQ_FLT( button.get_effective_minimum_size().y, preferred.y );
    // No explicit override anywhere above: the plain getters stay untouched.
    TESTLABS_CHECK_EQ_FLT( button.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( button.get_minimum_size().x, 0.0f );

    button.set_text(U"Much Longer Caption");
    TESTLABS_CHECK( button.get_effective_preferred_size().x > preferred.x );
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
