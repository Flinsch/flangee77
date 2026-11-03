
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/faces/TextField.h>

#include <XiaoLabs/Clipboard.h>

#include "../shared.h"



namespace {

    /**
     * Restores the clipboard's original text content once a test case ends (see
     * XiaoLabs/Clipboard.cpp): the Ctrl+C/X/V tests below necessarily touch the
     * real OS clipboard, so whatever was there before must come back afterward.
     */
    struct ClipboardRestoreGuard
    {
        cl7::u32string original_text;
        ~ClipboardRestoreGuard() { xl7::clipboard().set_text(original_text); }
    };

} // namespace



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  default-constructed is empty; the text constructor argument sets the initial text with the caret already at its end, without emitting get_changed()" )
{
    fl7::gui::faces::TextField empty_field;
    TESTLABS_CHECK( empty_field.get_text().empty() );
    TESTLABS_CHECK_EQ( empty_field.get_caret_codepoint_index(), size_t(0) );
    TESTLABS_CHECK( !empty_field.has_selection() );

    fl7::gui::faces::TextField field(U"Hello");
    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; }); // connected only after construction
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello") );
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(5) );
    TESTLABS_CHECK( !field.has_selection() );
    TESTLABS_CHECK_EQ( emit_count, 0 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  set_text() moves the caret to the end and clears any selection, emitting get_changed() only on an actual change" )
{
    fl7::gui::faces::TextField field(U"Hello");

    int emit_count = 0;
    cl7::u32string last_text;
    field.get_changed().connect([&](const cl7::u32string& text) { ++emit_count; last_text = text; });

    field.set_text(U"Hello"); // already that text: no-op
    TESTLABS_CHECK_EQ( emit_count, 0 );

    field.set_text(U"Goodbye");
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Goodbye") );
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(7) );
    TESTLABS_CHECK_EQ( emit_count, 1 );
    TESTLABS_CHECK_EQ( last_text, cl7::u32string(U"Goodbye") );
}



// #############################################################################
// Interaction (via a Shell and simulated mouse/keyboard input)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  clicking the field focuses it (the caret becomes visible); clicking empty space blurs it again" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello");
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 20.0f});

    TESTLABS_CHECK( !field.is_caret_visible() ); // not focused yet

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( field.is_caret_visible() );

    // Clicking on empty space (outside the field, hitting nothing) still
    // clears the stale focus, same as clicking some other face would.
    input.move_mouse_to({500.0f, 500.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( !field.is_caret_visible() );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  typing inserts text at the caret, emitting get_changed()" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>();
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; });

    // Delivered as one composed string per frame, same as WM_CHAR-style text
    // input (see Keyboard::get_text_input()), not one key event per character.
    input.type_text(U"Hello");
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello") );
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(5) );
    TESTLABS_CHECK_EQ( emit_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  Home/End move the caret to the start/end; Backspace/Delete remove the character behind/ahead of it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello"); // caret starts at the end (5)
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; });

    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(0) );

    input.set_key_down(xl7::input::Key::Delete, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Delete, false);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"ello") ); // removed the leading 'H'
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(0) );

    input.set_key_down(xl7::input::Key::End, true);
    shell.update();
    input.set_key_down(xl7::input::Key::End, false);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(4) );

    input.set_key_down(xl7::input::Key::Backspace, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Backspace, false);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"ell") ); // removed the trailing 'o'

    TESTLABS_CHECK_EQ( emit_count, 2 ); // only Delete/Backspace actually change the text, Home/End don't emit
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  holding Shift while pressing Left extends the selection instead of just moving the caret; a plain Right afterward collapses it to the selection's end" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello World"); // caret starts at the end (11)
    field.set_position({0.0f, 0.0f});
    field.set_size({200.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftShift, true);
    shell.update();
    for (int i = 0; i < 5; ++i)
    {
        input.set_key_down(xl7::input::Key::Left, true);
        shell.update();
        input.set_key_down(xl7::input::Key::Left, false);
        shell.update();
    }
    input.set_key_down(xl7::input::Key::LeftShift, false);
    shell.update();

    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(6) );
    TESTLABS_CHECK( field.has_selection() );
    TESTLABS_CHECK_EQ( field.get_selection_begin_codepoint_index(), size_t(6) );
    TESTLABS_CHECK_EQ( field.get_selection_end_codepoint_index(), size_t(11) ); // selects "World"

    input.set_key_down(xl7::input::Key::Right, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Right, false);
    shell.update();

    TESTLABS_CHECK( !field.has_selection() );
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(11) );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  Ctrl+A selects the entire text; typing over an active selection replaces it instead of just inserting" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello");
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::A, true);
    shell.update();
    input.set_key_down(xl7::input::Key::A, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();

    TESTLABS_CHECK( field.has_selection() );
    TESTLABS_CHECK_EQ( field.get_selection_begin_codepoint_index(), size_t(0) );
    TESTLABS_CHECK_EQ( field.get_selection_end_codepoint_index(), size_t(5) );

    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; });

    input.type_text(U"Hi");
    shell.update();

    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hi") ); // replaced the whole selection, not just inserted at its end
    TESTLABS_CHECK( !field.has_selection() );
    TESTLABS_CHECK_EQ( emit_count, 1 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  Ctrl+C/X/V copy/cut/paste the selection via the system clipboard" )
{
    TESTLABS_ASSERT( xl7::clipboard().init() );
    const ClipboardRestoreGuard restore_guard{ xl7::clipboard().get_text() };

    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello World"); // caret starts at the end (11)
    field.set_position({0.0f, 0.0f});
    field.set_size({200.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    // Select "World" (the last 5 code points) via Shift+Left.
    input.set_key_down(xl7::input::Key::LeftShift, true);
    shell.update();
    for (int i = 0; i < 5; ++i)
    {
        input.set_key_down(xl7::input::Key::Left, true);
        shell.update();
        input.set_key_down(xl7::input::Key::Left, false);
        shell.update();
    }
    input.set_key_down(xl7::input::Key::LeftShift, false);
    shell.update();

    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; });

    // Ctrl+C: copies the selection, leaves the field itself untouched.
    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::C, true);
    shell.update();
    input.set_key_down(xl7::input::Key::C, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();

    TESTLABS_CHECK_EQ( xl7::clipboard().get_text(), cl7::u32string(U"World") );
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello World") );
    TESTLABS_CHECK_EQ( emit_count, 0 );

    // Ctrl+X: cuts the selection, leaving just "Hello " behind.
    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::X, true);
    shell.update();
    input.set_key_down(xl7::input::Key::X, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();

    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello ") );
    TESTLABS_CHECK_EQ( emit_count, 1 );

    // Ctrl+V: pastes it right back at the caret, restoring the original text.
    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::V, true);
    shell.update();
    input.set_key_down(xl7::input::Key::V, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();

    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello World") );
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(11) );
    TESTLABS_CHECK_EQ( emit_count, 2 );
}
