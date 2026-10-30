
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/CheckBox.h>

#include "../shared.h"



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
