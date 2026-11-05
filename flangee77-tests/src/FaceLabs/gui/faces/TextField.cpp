
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Panel.h>
#include <FaceLabs/gui/faces/TextField.h>
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>

#include <XiaoLabs/Clipboard.h>

#include <CoreLabs/platform/filesystem.h>

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

    /**
     * A real, loaded font, for the scroll-bar tests below: unlike every test
     * above, those actually need get_effective_style().font to be non-null
     * (see the "Scrolling" section's own comment), which set_style_override()
     * (bypassing the theme/Shell entirely) makes possible even in this
     * NullRenderer-based harness.
     */
    fl7::fonts::Font& get_test_font()
    {
        static fl7::fonts::Font font(std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(
            cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf"));
        return font;
    }

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



// #############################################################################
// Multiline (set_multiline_enabled())
// #############################################################################
//
// Note: NullRenderer-based tests have no theme/font set up anywhere (see
// shared.h), so get_effective_style().font is always nullptr here, and every
// font-dependent helper (_lay_out() and everything built on it: 2D mouse hit-
// testing, Up/Down caret movement, line-based Home/End) falls back to its
// defensive no-font behavior instead of actually doing anything: same,
// pre-existing limitation single-line mouse-click hit-testing already had.
// That math is instead covered directly (with a real font) by
// FaceLabs/fonts/TextLayout.cpp's own locate()/lay_out_box() tests. What's
// covered here is what IS font-independent: the property itself, literal
// line-break insertion (pure text manipulation), Ctrl+Home/Ctrl+End (which
// bypass line lookup entirely), Backspace/Delete across a line break, and
// get_multiline_enabled() gating whether Up/Down are consumed at all.
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  get_multiline_enabled() defaults to false; set_multiline_enabled() toggles it" )
{
    fl7::gui::faces::TextField field;
    TESTLABS_CHECK( !field.get_multiline_enabled() );

    field.set_multiline_enabled(true);
    TESTLABS_CHECK( field.get_multiline_enabled() );

    field.set_multiline_enabled(false);
    TESTLABS_CHECK( !field.get_multiline_enabled() );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  with multiline enabled, a line break (CR or LF, same as Enter is reported, see Face::_on_text_input()) is inserted as a single LF; with it disabled (the default), it's dropped like any other control character" )
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

    // Disabled (the default): dropped, same as any other control character.
    input.type_text(U"\r");
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello") );

    // Enabled: both CR and LF turn into a single LF each.
    field.set_multiline_enabled(true);

    int emit_count = 0;
    field.get_changed().connect([&](const cl7::u32string&) { ++emit_count; });

    input.type_text(U"\r");
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello\n") );
    TESTLABS_CHECK_EQ( emit_count, 1 );

    input.type_text(U"\n");
    shell.update();
    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"Hello\n\n") );
    TESTLABS_CHECK_EQ( emit_count, 2 );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  Ctrl+Home/Ctrl+End always jump to the whole text's start/end, even with multiline enabled" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello\nWorld"); // set directly, bypassing _on_text_input()'s own filter
    field.set_multiline_enabled(true);
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 40.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();

    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(0) );
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();

    input.set_key_down(xl7::input::Key::End, true);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), field.get_text().size() );
    input.set_key_down(xl7::input::Key::End, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  Backspace/Delete remove a line break like any other single code point, joining the two lines" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hello\nWorld");
    field.set_multiline_enabled(true);
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 40.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();

    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, true); // jump to the very start first
    shell.update();
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();

    for (int i = 0; i < 5; ++i) // past "Hello", right up against the line break
    {
        input.set_key_down(xl7::input::Key::Right, true);
        shell.update();
        input.set_key_down(xl7::input::Key::Right, false);
        shell.update();
    }
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(5) );

    input.set_key_down(xl7::input::Key::Delete, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Delete, false);
    shell.update();

    TESTLABS_CHECK_EQ( field.get_text(), cl7::u32string(U"HelloWorld") ); // the line break itself is gone, joining the two lines
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  with multiline enabled, Up/Down stay with the field instead of moving focus elsewhere; without it (the default), they're free for navigation as before" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    // Both fields share one top-level ancestor: spatial arrow navigation (see
    // Shell::focus_toward()) never crosses between separate top-level faces.
    auto& window = shell.add_face<fl7::gui::faces::Panel>();
    window.set_position({0.0f, 0.0f});
    window.set_size({200.0f, 200.0f});

    auto& field = window.add_child<fl7::gui::faces::TextField>(U"Hello");
    field.set_position({0.0f, 0.0f});
    field.set_size({100.0f, 20.0f});

    auto& below = window.add_child<fl7::gui::faces::TextField>(U"World");
    below.set_position({0.0f, 40.0f});
    below.set_size({100.0f, 20.0f});

    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&field) );

    // Without multiline (the default): Down is free, focus moves to `below`.
    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&below) );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();

    // Back on `field`, now with multiline enabled: Down stays here instead.
    input.move_mouse_to({50.0f, 10.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    field.set_multiline_enabled(true);

    input.set_key_down(xl7::input::Key::Down, true);
    shell.update();
    TESTLABS_CHECK( shell.get_focused_face() == static_cast<fl7::gui::Face*>(&field) );
    input.set_key_down(xl7::input::Key::Down, false);
    shell.update();
}



// #############################################################################
// Scrolling (vertical, while get_multiline_enabled() and the content overflows)
// #############################################################################
//
// This is the one corner of this field's behavior that genuinely needs a real
// font to exercise meaningfully (see the "Multiline" section's own comment
// above): whether the scroll bar actually shows/narrows the text box, and
// whether the field ever scrolls at all, both come down to real line-height/
// line-count math against a real font. set_style_override() (bypassing the
// theme/Shell entirely) makes that possible even in this NullRenderer-based
// harness, without needing a whole Theme.
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  set_style_override() with a real font: the scroll bar only narrows the text box once its content actually overflows this field's own height" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::Style style;
    style.font = &get_test_font();
    style.text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hi");
    field.set_style_override(style);
    field.set_position({0.0f, 0.0f});
    field.set_size({200.0f, 60.0f});
    field.set_multiline_enabled(true);

    TESTLABS_ASSERT( field.get_text_box_size().has_value() );
    TESTLABS_CHECK_EQ_FLT( field.get_text_box_size()->x, field.get_size().x ); // fits: no scroll bar, full width

    cl7::u32string many_lines;
    for (int i = 0; i < 10; ++i)
        many_lines += U"Line\n";
    field.set_text(many_lines);

    TESTLABS_ASSERT( field.get_text_box_size().has_value() );
    TESTLABS_CHECK_LT( field.get_text_box_size()->x, field.get_size().x ); // overflows: scroll bar narrows it

    field.set_text(U"Hi"); // back to fitting: widens again
    TESTLABS_CHECK_EQ_FLT( field.get_text_box_size()->x, field.get_size().x );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  set_style_override() with a real font: the mouse wheel scrolls this field's own content while it's actually overflowing, but does nothing while it isn't" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::Style style;
    style.font = &get_test_font();
    style.text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;

    auto& field = shell.add_face<fl7::gui::faces::TextField>(U"Hi");
    field.set_style_override(style);
    field.set_position({0.0f, 0.0f});
    field.set_size({200.0f, 60.0f});
    field.set_multiline_enabled(true);

    input.move_mouse_to({100.0f, 30.0f});
    shell.update();

    // Not overflowing yet: the wheel has nothing to do.
    input.scroll_wheel(-1);
    shell.update();
    TESTLABS_CHECK_EQ_FLT( field.get_text_scroll_offset().y, 0.0f );

    cl7::u32string many_lines;
    for (int i = 0; i < 10; ++i)
        many_lines += U"Line\n";
    field.set_text(many_lines);

    // set_text() itself moves the caret to the very end, which, now that the
    // field overflows, already auto-scrolls it all the way down (see
    // _ensure_caret_visible()). Jump the caret (and scroll position) back to
    // the very start first, so the wheel notches below start from 0. Needs
    // an actual click first: Ctrl+Home only reaches this field while it's
    // focused, which hovering it alone (above) never did.
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();
    TESTLABS_CHECK_EQ_FLT( field.get_text_scroll_offset().y, 0.0f );

    // Now overflowing: a notch "away from the user" scrolls the content down,
    // and further notches keep scrolling it further down still.
    input.scroll_wheel(-1);
    shell.update();
    TESTLABS_CHECK_GT( field.get_text_scroll_offset().y, 0.0f );

    const float after_one_notch = field.get_text_scroll_offset().y;
    input.scroll_wheel(-1);
    shell.update();
    TESTLABS_CHECK_GT( field.get_text_scroll_offset().y, after_one_notch );

    // Scrolling back up the other way clamps back to exactly 0, never past it.
    for (int i = 0; i < 20; ++i)
    {
        input.scroll_wheel(1);
        shell.update();
    }
    TESTLABS_CHECK_EQ_FLT( field.get_text_scroll_offset().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  TextField:  set_style_override() with a real font: enabling multiline with the caret already out of view, or moving it there with Down, scrolls it back into view automatically" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::Style style;
    style.font = &get_test_font();
    style.text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;

    cl7::u32string many_lines;
    for (int i = 0; i < 20; ++i)
        many_lines += U"Line\n";

    // The constructor leaves the caret at the very end (see TextField(text)'s
    // own doc comment), i.e., on the very last of these 20 (or 21, counting
    // the empty line after the final '\n') lines.
    auto& field = shell.add_face<fl7::gui::faces::TextField>(many_lines);
    field.set_style_override(style);
    field.set_position({0.0f, 0.0f});
    field.set_size({200.0f, 40.0f}); // nowhere near tall enough for all those lines

    // Enabling multiline with the caret already past the (now relevant)
    // visible area scrolls it into view right away, without needing a
    // further nudge first.
    field.set_multiline_enabled(true);
    TESTLABS_CHECK_GT( field.get_text_scroll_offset().y, 0.0f );

    // Jump back to the very start: scrolls all the way back up.
    input.move_mouse_to({100.0f, 20.0f});
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, true);
    shell.update();
    input.set_key_down(xl7::input::Key::Home, false);
    shell.update();
    input.set_key_down(xl7::input::Key::LeftControl, false);
    shell.update();
    TESTLABS_CHECK_EQ( field.get_caret_codepoint_index(), size_t(0) );
    TESTLABS_CHECK_EQ_FLT( field.get_text_scroll_offset().y, 0.0f );

    // Down, repeatedly, past the visible lines: never scrolls back up on its
    // own, and does end up scrolling further down at least once.
    float previous_offset = field.get_text_scroll_offset().y;
    bool ever_scrolled_further = false;
    for (int i = 0; i < 19; ++i)
    {
        input.set_key_down(xl7::input::Key::Down, true);
        shell.update();
        input.set_key_down(xl7::input::Key::Down, false);
        shell.update();

        const float offset = field.get_text_scroll_offset().y;
        TESTLABS_CHECK( offset >= previous_offset );
        ever_scrolled_further = ever_scrolled_further || offset > previous_offset;
        previous_offset = offset;
    }
    TESTLABS_CHECK( ever_scrolled_further );
    TESTLABS_CHECK_GT( field.get_caret_codepoint_index(), size_t(0) );
}
