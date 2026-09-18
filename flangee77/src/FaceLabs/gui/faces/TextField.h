#ifndef FL7_GUI_FACES_TEXTFIELD_H
#define FL7_GUI_FACES_TEXTFIELD_H
#include "../Control.h"
#include "../HasBackground.h"
#include "../HasCaret.h"
#include "../HasSelection.h"
#include "../HasText.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>

#include <algorithm>



namespace fl7::gui::faces {



/**
 * A single-line, editable piece of text: click to position the caret, type to
 * insert at it, move it (Left/Right/Home/End), or delete around it (Backspace/
 * Delete). Also supports selecting a range, either by dragging the mouse or by
 * holding Shift while moving the caret, which then gets replaced/deleted as a whole
 * by typing/Backspace/Delete, same as it would in any other text editor, or
 * copied/cut/pasted via the system clipboard with Ctrl+C/X/V. No wrapping/
 * scrolling, though: text that overflows the field's width is simply clipped,
 * same as any other face's (see Collection's clipping).
 */
class TextField
    : public Control
    , public HasBackground
    , public HasText
    , public HasCaret
    , public HasSelection
{

public:
    TextField() = default;

    explicit TextField(cl7::u32string text)
        : _text(std::move(text))
        , _caret_index(_text.size())
        , _selection_anchor(_caret_index)
    {
    }

    TextField(const TextField&) = delete;
    TextField& operator=(const TextField&) = delete;
    TextField(TextField&&) = delete;
    TextField& operator=(TextField&&) = delete;

    ~TextField() override = default;



    // #############################################################################
    // HasText Implementations
    // #############################################################################

    /** Returns this text field's text to be displayed. */
    const cl7::u32string& get_display_text() const override { return _text; }



    // #############################################################################
    // HasCaret Implementations
    // #############################################################################

    /** Returns whether this text field is currently focused (the only time its caret is drawn). */
    bool is_caret_visible() const override;

    /** Returns the code point index the caret is currently positioned at. */
    size_t get_caret_codepoint_index() const override { return _caret_index; }



    // #############################################################################
    // HasSelection Implementations
    // #############################################################################

    /** Returns whether there's currently a non-empty selection. */
    bool has_selection() const override { return _selection_anchor != _caret_index; }

    /** Returns the code point index the selection starts at. */
    size_t get_selection_begin_codepoint_index() const override { return std::min(_selection_anchor, _caret_index); }

    /** Returns the code point index the selection ends at (exclusive). */
    size_t get_selection_end_codepoint_index() const override { return std::max(_selection_anchor, _caret_index); }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this text field's text. */
    const cl7::u32string& get_text() const { return _text; }

    /** Sets this text field's text, unless already that text; moves the caret to its end, and clears any selection. */
    void set_text(cl7::u32string text);

    /** Returns the signal emitted (with the new text) whenever this text field's text actually changes. */
    cl7::behavioral::Signal<const cl7::u32string&>& get_changed() { return _changed; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this text field's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"text_field"; }

    /** An editable field must always show exactly what was typed, so this is non-negotiable, not theme-driven. */
    void _adjust_effective_text_style(fonts::TextStyle& text_style) const override { text_style.preserve_whitespace = true; }

    /** Positions the caret at the clicked code point, and starts a fresh (empty) selection there. */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /** Extends the selection from wherever the drag started to the code point now under the cursor. */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;

    /**
     * Moves the caret, or deletes the code point before/after it (or, with an
     * active selection, deletes the whole selection instead). Shift+Left/Right/
     * Home/End extends the selection instead of moving/collapsing it. Ctrl+C/X/V
     * copies/cuts/pastes the selection via the system clipboard (see
     * xl7::Clipboard).
     */
    void _on_key_down(xl7::input::Key key) override;

    /** Inserts the input (filtered down to plain code points) at the caret, replacing any active selection. */
    void _on_text_input(const cl7::u32string& text) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /** Returns the code point index within _text whose caret position is closest to local_x. */
    size_t _codepoint_index_at(float local_x) const;

    /** Returns whether either Shift key is currently held. */
    bool _is_shift_down() const;

    /** Returns whether either Control key is currently held. */
    bool _is_ctrl_down() const;

    /** Erases the current selection from _text, and collapses the caret (and selection) to where it began. */
    void _delete_selection();



    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u32string _text;

    /** Into _text, in [0, _text.size()]. */
    size_t _caret_index = 0;

    /** Into _text, in [0, _text.size()]; the selection's fixed end. Equal to _caret_index means no selection. */
    size_t _selection_anchor = 0;

    cl7::behavioral::Signal<const cl7::u32string&> _changed;

}; // class TextField



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_TEXTFIELD_H
