#ifndef FL7_GUI_FACES_TEXTFIELD_H
#define FL7_GUI_FACES_TEXTFIELD_H
#include "../Compound.h"
#include "../HasBackground.h"
#include "../HasCaret.h"
#include "../HasSelection.h"
#include "../HasText.h"
#include "../ScrollBarVisibility.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>

#include <algorithm>
#include <optional>



namespace fl7::gui::faces {



class ScrollBar; // Used as this field's own vertical scroll bar, while get_multiline_enabled() and its content overflows; defined in ScrollBar.h, not needed here.



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
 * field's width is simply clipped (see AbstractRenderer::_draw_face()), and
 * Enter does nothing (consumed by Shell for focus activation instead, see
 * Shell::update()). set_multiline_enabled(true) opts into word-wrapping and
 * literal Enter/line-break support, plus Up/Down moving the caret a line at a
 * time and Home/End meaning the current LINE's start/end rather than the
 * whole text's (Ctrl+Home/Ctrl+End still jump to the very start/end
 * regardless), plus a vertical scroll bar of its own (see Compound; a fixed
 * composition, same as ScrollBar's own track + buttons), shown, by default,
 * only while the text actually overflows this field's own height (see
 * gui::ScrollBarVisibility/set_scroll_bar_visibility()), that keeps the caret
 * scrolled into view automatically as it moves (see _ensure_caret_visible()),
 * and that the mouse wheel also scrolls while hovered. Note: the scroll bar
 * being its own genuine Tab stop (same as, e.g., faces::ScrollPanel's own
 * scroll bars) means it, not just this field itself, can end up focused.
 */
class TextField
    : public Compound
    , public HasBackground
    , public HasText
    , public HasCaret
    , public HasSelection
{

public:
    TextField();

    explicit TextField(cl7::u32string text);

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

    /** Returns {0, current vertical scroll position}; see get_multiline_enabled(). */
    ml7::Vector2f get_text_scroll_offset() const override { return {0.0f, _scroll_offset_y}; }

    /** Returns this field's own size, narrowed by the scroll bar's width while it's shown; see get_multiline_enabled(). */
    std::optional<ml7::Vector2f> get_text_box_size() const override { return _effective_text_box_size(); }



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

    /**
     * Sets whether this text field wraps/accepts literal line breaks, unless
     * already that; re-lays-out its scroll bar accordingly (see _relayout()) and
     * scrolls the caret into view if necessary.
     */
    void set_multiline_enabled(bool enabled);

    /** Returns this text field's scroll bar's visibility mode; see set_scroll_bar_visibility(). Default: Auto. */
    ScrollBarVisibility get_scroll_bar_visibility() const { return _scroll_bar_visibility; }

    /**
     * Sets this text field's scroll bar's visibility mode, unless already that;
     * re-lays-out accordingly (see _relayout()) and scrolls the caret into view
     * if necessary. Has no effect while !get_multiline_enabled() (there's no
     * scroll bar to show at all then, regardless).
     */
    void set_scroll_bar_visibility(ScrollBarVisibility visibility);



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
     * field's own caret/selection/hit-testing math (see _lay_out()). While
     * actually scrolling (the scroll bar is shown; see _relayout()),
     * vertical_align is additionally pinned to Top: Middle/Bottom would
     * otherwise position the (now overflowing) text block so that even its
     * very first line starts above this field's own top edge at scroll
     * position 0, which isn't what "scrolled all the way up" should look
     * like. Left alone (not actually overflowing yet), whatever the theme/
     * override sets is honored as-is, same as a single-line field's.
     */
    void _adjust_effective_text_style(fonts::TextStyle& text_style) const override;

    /**
     * Keeps the scroll bar's size/position, and its range/visible_fraction/step,
     * in sync with this field's own new size (see _relayout()), and scrolls the
     * caret into view if necessary.
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /**
     * Positions the caret at the clicked code point, and starts a fresh (empty)
     * selection there; scrolls it into view if necessary.
     */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /**
     * Extends the selection from wherever the drag started to the code point now
     * under the cursor; scrolls it into view if necessary.
     */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;

    /**
     * Scrolls vertically by the scroll bar's own step per wheel notch, while
     * get_multiline_enabled() and the scroll bar is actually shown. Always
     * consumes then (even already clamped at an end), never otherwise.
     */
    bool _on_mouse_wheel(int delta) override;

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
     * Scrolls the caret into view afterward, if necessary.
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

    /** A text field as a whole is an interactive leaf control, so it's a Tab stop (Compound itself doesn't opt into that by default). */
    bool _is_focusable() const override { return true; }



private:

    // #############################################################################
    // Constants
    // #############################################################################

    /**
     * Fixed for now, should eventually come from the active theme/chrome
     * instead (e.g., the 9-slice chrome's own native scroll bar thickness);
     * see faces::ScrollPanel's own identical constant/caveat.
     */
    static constexpr float SCROLLBAR_THICKNESS = 16.0f;



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
     * Returns this field's own size, narrowed by SCROLLBAR_THICKNESS while
     * get_multiline_enabled() and the scroll bar is actually shown (see
     * _relayout(), the only place that decides that); unchanged otherwise.
     */
    ml7::Vector2f _effective_text_box_size() const;

    /**
     * Re-decides (while get_multiline_enabled() and there's a font to lay out
     * against) whether the scroll bar is actually shown, per
     * get_scroll_bar_visibility(): Always unconditionally; Auto only once this
     * field's content actually overflows its own height at its full width, or,
     * if not yet, once more at the width actually left over once the scroll bar
     * takes SCROLLBAR_THICKNESS off of it (a narrower width only ever wraps to
     * more, never fewer, lines, so this can't flip back off and one re-check is
     * enough to converge, same reasoning as faces::ScrollPanel's own
     * single-axis visibility resolution); Never, well, never. Either way, sizes/
     * positions the scroll bar accordingly (always, even hidden, so it's ready
     * to reappear at the right place/size), and updates its range/
     * visible_fraction/step (one line) from the result, re-clamping the current
     * scroll position to it (see _set_scroll_offset()). While
     * !get_multiline_enabled() or there's no font yet, simply hides/disables
     * the scroll bar and resets the scroll position to 0 instead, regardless of
     * get_scroll_bar_visibility().
     */
    void _relayout();

    /**
     * Sets _scroll_offset_y, clamped to the scroll bar's own current range, and
     * syncs the scroll bar's own value to match.
     */
    void _set_scroll_offset(float scroll_offset_y);

    /**
     * Scrolls just enough (if at all) to bring the caret's own current line
     * back within view, while get_multiline_enabled(). A no-op otherwise, or
     * if it's already (at least partially) visible.
     */
    void _ensure_caret_visible();

    /**
     * Runs _relayout() and _ensure_caret_visible(), then emits get_changed();
     * every place _text actually changes funnels through this, instead of
     * emitting directly.
     */
    void _on_text_changed();

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

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    ScrollBar& _scroll_bar;

    cl7::u32string _text;

    /** Into _text, in [0, _text.size()]. */
    size_t _caret_index = 0;

    /** Into _text, in [0, _text.size()]; the selection's fixed end. Equal to _caret_index means no selection. */
    size_t _selection_anchor = 0;

    bool _multiline_enabled = false;

    /** See get_scroll_bar_visibility()/set_scroll_bar_visibility(). */
    ScrollBarVisibility _scroll_bar_visibility = ScrollBarVisibility::Auto;

    /** How far this field's content is currently scrolled down, in pixels; always 0 while !_multiline_enabled. See _relayout()/_set_scroll_offset(). */
    float _scroll_offset_y = 0.0f;

    cl7::behavioral::Signal<const cl7::u32string&> _changed;

}; // class TextField



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_TEXTFIELD_H
