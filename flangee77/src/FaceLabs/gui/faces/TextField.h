#ifndef FL7_GUI_FACES_TEXTFIELD_H
#define FL7_GUI_FACES_TEXTFIELD_H
#include "../Control.h"
#include "../HasBackground.h"
#include "../HasCaret.h"
#include "../HasSelection.h"
#include "../HasText.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>

#include <algorithm>
#include <optional>



namespace fl7::gui::faces {



/**
 * An editable piece of text: click to position the caret, type to insert at
 * it, move it (Left/Right/Home/End, or word-at-a-time with Ctrl+Left/Right),
 * or delete around it (Backspace/Delete). Also supports selecting a range,
 * either by dragging the mouse, by holding Shift while moving the caret
 * (word-at-a-time too, combined with Ctrl), or all at once with Ctrl+A. A
 * selection then gets replaced/deleted as a whole by typing/Backspace/Delete,
 * same as it would in any other text editor, or copied/cut/pasted via the
 * system clipboard with Ctrl+C/X/V.
 *
 * Single-line by default: no wrapping/scrolling, text that overflows the
 * field's width is simply clipped (same as any other face's, see Collection's
 * clipping), and Enter does nothing (consumed by Shell for focus activation
 * instead, see Shell::update()). set_multiline_enabled(true) opts into word-
 * wrapping and literal Enter/line-break support, plus Up/Down moving the caret
 * a line at a time and Home/End meaning the current LINE's start/end rather
 * than the whole text's (Ctrl+Home/Ctrl+End still jump to the very start/end
 * regardless). Still no scrolling, though: a multiline field taller than its
 * own box still just clips, same as before.
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

    /**
     * Returns whether this text field is currently focused (the only time its caret
     * is drawn).
     */
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

    /**
     * Sets this text field's text, unless already that text; moves the caret to its
     * end, and clears any selection.
     */
    void set_text(cl7::u32string text);

    /** Returns the signal emitted (with the new text) whenever this text field's text actually changes. */
    cl7::behavioral::Signal<const cl7::u32string&>& get_changed() { return _changed; }

    /** Returns whether this text field currently wraps/accepts literal line breaks (see the class doc comment). Default: false. */
    bool get_multiline_enabled() const { return _multiline_enabled; }

    /** Sets whether this text field wraps/accepts literal line breaks. */
    void set_multiline_enabled(bool enabled) { _multiline_enabled = enabled; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this text field's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"text_field"; }

    /**
     * An editable field must always show exactly what was typed, so
     * preserve_whitespace is non-negotiable, not theme-driven; wrap_mode
     * likewise always follows get_multiline_enabled() directly, regardless of
     * whatever a theme happens to set, so rendering always matches this
     * field's own caret/selection/hit-testing math (see _lay_out()).
     */
    void _adjust_effective_text_style(fonts::TextStyle& text_style) const override
    {
        text_style.preserve_whitespace = true;
        text_style.wrap_mode = _multiline_enabled ? fonts::TextStyle::WrapMode::Word : fonts::TextStyle::WrapMode::None;
    }

    /**
     * Positions the caret at the clicked code point, and starts a fresh (empty)
     * selection there.
     */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /**
     * Extends the selection from wherever the drag started to the code point now
     * under the cursor.
     */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;

    /**
     * Moves the caret, or deletes the code point before/after it (or, with an
     * active selection, deletes the whole selection instead). Ctrl+Left/Right
     * moves a whole word at a time instead of one code point. Shift+Left/Right/
     * Home/End/Ctrl+Left/Ctrl+Right extends the selection instead of moving/
     * collapsing it; Ctrl+A selects everything outright. Ctrl+C/X/V copies/cuts/
     * pastes the selection via the system clipboard (see xl7::Clipboard). While
     * get_multiline_enabled(): Up/Down move the caret a line at a time (keeping
     * roughly the same column); Home/End jump to the current LINE's start/end
     * rather than the whole text's (Ctrl+Home/Ctrl+End still do, regardless).
     */
    void _on_key_down(xl7::input::Key key) override;

    /**
     * Inserts the input (filtered down to plain code points, plus line breaks while
     * get_multiline_enabled()) at the caret, replacing any active selection.
     */
    void _on_text_input(const cl7::u32string& text) override;

    /**
     * Consumes the horizontal arrows for the text caret (soft), so plain
     * arrow-key focus navigation yields while forced navigation (gamepad,
     * Alt+arrow) can still leave; same for the vertical arrows, but only
     * while get_multiline_enabled() (there's no vertical caret movement
     * otherwise, so they stay free for navigation, as before).
     */
    ArrowResponse _consumes_arrow(ArrowDirection dir) const override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /** _text, already decoded, laid out against this field's own current size/style (see fonts::TextLayout::lay_out_box()); bundled since every caret/selection/hit-testing helper below needs all three together. */
    struct _Layout
    {
        Style style;
        std::vector<cl7::text::codec::codepoint> codepoints;
        fonts::TextBlockLayout block;
    };

    /**
     * Returns _Layout for _text, or empty if get_effective_style() has no font to
     * lay out against (yet).
     */
    std::optional<_Layout> _lay_out() const;

    /**
     * Returns the index, within `line`, of the code point whose caret position
     * is closest to target_x (relative to the line's own start).
     */
    static size_t _nearest_codepoint_in_line(const _Layout& layout, const fonts::TextLine& line, float target_x);

    /**
     * Returns the index into `lines` of the line that `codepoint_index` falls
     * on: the last one whose own start still reaches it, same convention as
     * fonts::TextLayout::locate() (an index sitting exactly on a line break
     * lands on the following line, not the end of the one before it).
     */
    static size_t _line_index_at(const std::vector<fonts::TextLine>& lines, size_t codepoint_index);

    /**
     * Returns the code point index within _text whose caret position is closest to
     * local_position (see _nearest_codepoint_in_line(), scoped to whichever line
     * local_position.y falls on).
     */
    size_t _codepoint_index_at(ml7::Vector2f local_position) const;

    /**
     * Moves _caret_index to the nearest code point, roughly the same column
     * over, on the line `line_delta` lines away; a no-op if there's no such
     * line (already the first/last one).
     */
    void _move_caret_vertically(int line_delta);

    /**
     * Returns the start of the line `codepoint_index` falls on, or 0 if there's no
     * font to lay out against (yet).
     */
    size_t _line_start(size_t codepoint_index) const;

    /**
     * Returns the end of the line `codepoint_index` falls on, or _text.size() if
     * there's no font to lay out against (yet).
     */
    size_t _line_end(size_t codepoint_index) const;

    /**
     * Returns whether either Shift key is currently held.
     */
    bool _is_shift_down() const;

    /**
     * Returns whether either Control key is currently held.
     */
    bool _is_ctrl_down() const;

    /**
     * Returns the start of the word at/before index (skipping any whitespace right before it first).
     */
    size_t _previous_word_boundary(size_t index) const;

    /**
     * Returns the start of the next word after index (skipping any whitespace right
     * after it first, then the rest of the current word, if any, then whatever
     * whitespace separates it from that next word), or _text.size().
     */
    size_t _next_word_boundary(size_t index) const;

    /**
     * Erases the current selection from _text, and collapses the caret (and
     * selection) to where it began.
     */
    void _delete_selection();



    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u32string _text;

    /** Into _text, in [0, _text.size()]. */
    size_t _caret_index = 0;

    /** Into _text, in [0, _text.size()]; the selection's fixed end. Equal to _caret_index means no selection. */
    size_t _selection_anchor = 0;

    bool _multiline_enabled = false;

    cl7::behavioral::Signal<const cl7::u32string&> _changed;

}; // class TextField



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_TEXTFIELD_H
