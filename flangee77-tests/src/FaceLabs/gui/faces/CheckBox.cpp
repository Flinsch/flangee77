
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/CheckBox.h>
#include <FaceLabs/gui/faces/Label.h>

#include "../../shared.h"
#include "../shared.h"



namespace {

    using f77_tests::fonts::get_test_font;

} // namespace



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  default-constructed is unchecked; the checked/text constructor arguments set the initial state without emitting get_changed()" )
{
    fl7::gui::faces::CheckBox default_box;
    TESTLABS_CHECK( !default_box.is_checked() );
    TESTLABS_CHECK( default_box.get_text().empty() );

    fl7::gui::faces::CheckBox box(true, U"Caption");
    int emit_count = 0;
    box.get_changed().connect([&](bool) { ++emit_count; }); // connected only after construction
    TESTLABS_CHECK( box.is_checked() );
    TESTLABS_CHECK_EQ( box.get_text(), cl7::u32string(U"Caption") );
    TESTLABS_CHECK_EQ( emit_count, 0 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  set_checked() only emits get_changed() on an actual change" )
{
    fl7::gui::faces::CheckBox box;

    int emit_count = 0;
    bool last_value = false;
    box.get_changed().connect([&](bool checked) { ++emit_count; last_value = checked; });

    box.set_checked(true);
    TESTLABS_CHECK( box.is_checked() );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK( last_value );

    box.set_checked(true); // already there: no-op
    TESTLABS_CHECK_EQ( emit_count, 1 );

    box.set_checked(false);
    TESTLABS_CHECK( !box.is_checked() );
    TESTLABS_CHECK_EQ( emit_count, 2 );
    TESTLABS_CHECK( !last_value );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  set_text()/get_text()" )
{
    fl7::gui::faces::CheckBox box;
    box.set_text(U"Hello");
    TESTLABS_CHECK_EQ( box.get_text(), cl7::u32string(U"Hello") );
}



// #############################################################################
// Preferred/minimum size (box glyph + gap + caption's own text-based size)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  without an explicit override, get_preferred_size()/get_minimum_size() stay {0, 0} regardless of font; get_effective_preferred_size()/get_effective_minimum_size() need the caption to have an effective font to calculate anything, else also {0, 0}" )
{
    fl7::gui::faces::CheckBox box(false, U"Caption");
    TESTLABS_CHECK_EQ_FLT( box.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_minimum_size().x, 0.0f );

    TESTLABS_CHECK_EQ_FLT( box.get_effective_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_effective_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_effective_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_effective_minimum_size().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  set_style_override() with a real font: get_effective_preferred_size()/get_effective_minimum_size() reflect the box glyph (square, side = caption height) plus a fixed gap plus the caption's own natural (unwrapped) size, growing with more text; get_preferred_size()/get_minimum_size() stay {0, 0} regardless (no explicit override set); the caption (a child Label) inherits the override from its CheckBox parent, same as any other nested style override" )
{
    constexpr float caption_gap = 6.0f; // CheckBox::CAPTION_GAP; mirrored here, it's private.

    fl7::gui::Style style;
    style.font = &get_test_font();

    // An unparented reference Label under the same override, to independently
    // compute what the caption's own natural size should be.
    fl7::gui::faces::Label reference_label(U"Caption");
    reference_label.set_style_override(style);
    const ml7::Vector2f caption_size = reference_label.get_effective_preferred_size();
    TESTLABS_CHECK( caption_size.x > 0.0f );
    TESTLABS_CHECK( caption_size.y > 0.0f );

    fl7::gui::faces::CheckBox box(false, U"Caption");
    box.set_style_override(style); // the caption Label child inherits this, same as any nested override.

    const ml7::Vector2f preferred = box.get_effective_preferred_size();
    TESTLABS_CHECK_EQ_FLT( preferred.x, caption_size.y + caption_gap + caption_size.x );
    TESTLABS_CHECK_EQ_FLT( preferred.y, caption_size.y );
    // The box glyph's own minimum doesn't constrain this further: same formula either way.
    TESTLABS_CHECK_EQ_FLT( box.get_effective_minimum_size().x, preferred.x );
    TESTLABS_CHECK_EQ_FLT( box.get_effective_minimum_size().y, preferred.y );
    // No explicit override anywhere above: the plain getters stay untouched.
    TESTLABS_CHECK_EQ_FLT( box.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_minimum_size().x, 0.0f );

    box.set_text(U"Much Longer Caption");
    TESTLABS_CHECK( box.get_effective_preferred_size().x > preferred.x );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  CheckBox:  clicking anywhere across the box glyph or the caption toggles the checked state, since both children are disabled and resolve to the CheckBox as a whole" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& box = shell.add_face<fl7::gui::faces::CheckBox>(false, U"Caption");
    box.set_position({0.0f, 0.0f});
    box.set_size({100.0f, 20.0f});

    // The box glyph is a 20x20 square at local x [0, 20); clicking inside it...
    input.move_mouse_to({10.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( box.is_checked() );

    // ...and clicking inside the caption area (local x [26, 100)) both resolve
    // to the CheckBox as a whole: neither child swallows the hit, since both
    // are disabled.
    input.move_mouse_to({60.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( !box.is_checked() );
}
