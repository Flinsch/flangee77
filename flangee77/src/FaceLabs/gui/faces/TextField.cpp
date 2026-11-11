#include "TextField.h"

#include "./ScrollBar.h"

#include "../Shell.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <XiaoLabs/Clipboard.h>

#include <CoreLabs/text/inspect.h>

#include <algorithm>
#include <span>



namespace fl7::gui::faces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    TextField::TextField()
        : _scroll_bar(add_child<ScrollBar>(ScrollBar::Orientation::Vertical))
    {
        _scroll_bar.set_visible(false);
        _scroll_bar.set_enabled(false);
        _scroll_bar.get_changed().connect([this](float value) { _set_scroll_offset(value); });
    }

    TextField::TextField(cl7::u32string text)
        : TextField()
    {
        _text = std::move(text);
        _caret_index = _text.size();
        _selection_anchor = _caret_index;
    }



    // #############################################################################
    // HasCaret Implementations
    // #############################################################################

    /**
     * Returns whether this text field is currently focused (the only time its caret
     * is drawn).
     */
    bool TextField::is_caret_visible() const
    {
        const Shell* shell = get_shell();
        return shell && shell->get_focused_face() == this;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets this text field's text, unless already that text; moves the caret to its
     * end, and clears any selection.
     */
    void TextField::set_text(cl7::u32string text)
    {
        if (_text == text)
            return;

        _text = std::move(text);
        _caret_index = _text.size();
        _selection_anchor = _caret_index;
        _on_text_changed();
    }

    /**
     * Sets whether this text field wraps/accepts literal line breaks, unless
     * already that; re-lays-out its scroll bar accordingly (see _relayout()) and
     * scrolls the caret into view if necessary.
     */
    void TextField::set_multiline_enabled(bool enabled)
    {
        if (enabled == _multiline_enabled)
            return;

        _multiline_enabled = enabled;
        _relayout();
        _ensure_caret_visible();
    }

    /**
     * Sets this text field's scroll bar's visibility mode, unless already that;
     * re-lays-out accordingly (see _relayout()) and scrolls the caret into view
     * if necessary. Has no effect while !get_multiline_enabled() (there's no
     * scroll bar to show at all then, regardless).
     */
    void TextField::set_scroll_bar_visibility(ScrollBarVisibility visibility)
    {
        if (visibility == _scroll_bar_visibility)
            return;

        _scroll_bar_visibility = visibility;
        _relayout();
        _ensure_caret_visible();
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

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
    void TextField::_adjust_effective_text_style(fonts::TextStyle& text_style) const
    {
        text_style.preserve_whitespace = true;
        text_style.wrap_mode = _multiline_enabled ? fonts::TextStyle::WrapMode::Word : fonts::TextStyle::WrapMode::None;
        if (_multiline_enabled && _scroll_bar.is_visible())
            text_style.vertical_align = fonts::TextStyle::VerticalAlign::Top;
    }

    /**
     * Keeps the scroll bar's size/position, and its range/visible_fraction/step,
     * in sync with this field's own new size (see _relayout()), and scrolls the
     * caret into view if necessary.
     */
    void TextField::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _relayout();
        _ensure_caret_visible();
    }

    /**
     * Positions the caret at the clicked code point, and starts a fresh (empty)
     * selection there; scrolls it into view if necessary.
     */
    void TextField::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position);
        _selection_anchor = _caret_index;
        _ensure_caret_visible();
    }

    /**
     * Extends the selection from wherever the drag started to the code point now
     * under the cursor; scrolls it into view if necessary.
     */
    void TextField::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position);
        _ensure_caret_visible();
    }

    /**
     * Scrolls vertically by the scroll bar's own step per wheel notch, while
     * get_multiline_enabled() and the scroll bar is actually shown. Always
     * consumes then (even already clamped at an end), never otherwise.
     */
    bool TextField::_on_mouse_wheel(int delta)
    {
        if (!_scroll_bar.is_visible())
            return false;

        _set_scroll_offset(_scroll_offset_y - static_cast<float>(delta) * _scroll_bar.get_step());
        return true;
    }

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
    void TextField::_on_key_down(xl7::input::Key key)
    {
        using xl7::input::Key;

        const bool shift = _is_shift_down();
        const bool ctrl = _is_ctrl_down();

        switch (key)
        {
        case Key::A:
            if (ctrl)
            {
                _selection_anchor = 0;
                _caret_index = _text.size();
            }
            break;

        case Key::C:
            if (ctrl && has_selection())
                xl7::clipboard().set_text(_text.substr(get_selection_begin_codepoint_index(), get_selection_end_codepoint_index() - get_selection_begin_codepoint_index()));
            break;

        case Key::X:
            if (ctrl && has_selection())
            {
                xl7::clipboard().set_text(_text.substr(get_selection_begin_codepoint_index(), get_selection_end_codepoint_index() - get_selection_begin_codepoint_index()));
                _delete_selection();
                _on_text_changed();
            }
            break;

        case Key::V:
            if (ctrl)
                _on_text_input(xl7::clipboard().get_text());
            break;

        case Key::Left:
            if (!shift && has_selection())
                _caret_index = get_selection_begin_codepoint_index();
            else if (ctrl)
                _caret_index = _previous_word_boundary(_caret_index);
            else if (_caret_index > 0)
                --_caret_index;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::Right:
            if (!shift && has_selection())
                _caret_index = get_selection_end_codepoint_index();
            else if (ctrl)
                _caret_index = _next_word_boundary(_caret_index);
            else if (_caret_index < _text.size())
                ++_caret_index;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::Up:
            if (_multiline_enabled)
            {
                _move_caret_vertically(-1);
                if (!shift)
                    _selection_anchor = _caret_index;
            }
            break;

        case Key::Down:
            if (_multiline_enabled)
            {
                _move_caret_vertically(1);
                if (!shift)
                    _selection_anchor = _caret_index;
            }
            break;

        case Key::Home:
            _caret_index = (_multiline_enabled && !ctrl) ? _line_start(_caret_index) : 0;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::End:
            _caret_index = (_multiline_enabled && !ctrl) ? _line_end(_caret_index) : _text.size();
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::Backspace:
            if (has_selection())
            {
                _delete_selection();
                _on_text_changed();
            }
            else if (_caret_index > 0)
            {
                _text.erase(_caret_index - 1, 1);
                --_caret_index;
                _selection_anchor = _caret_index;
                _on_text_changed();
            }
            break;

        case Key::Delete:
            if (has_selection())
            {
                _delete_selection();
                _on_text_changed();
            }
            else if (_caret_index < _text.size())
            {
                _text.erase(_caret_index, 1);
                _selection_anchor = _caret_index;
                _on_text_changed();
            }
            break;

        default:
            break;
        }

        _ensure_caret_visible();
    }

    /**
     * Inserts the input (filtered down to plain code points, plus line breaks while
     * get_multiline_enabled()) at the caret, replacing any active selection.
     */
    void TextField::_on_text_input(const cl7::u32string& text)
    {
        cl7::u32string filtered;
        filtered.reserve(text.size());
        for (char32_t c : text)
        {
            // CR and LF (Enter is reported as one or the other, see Face.h's own
            // doc comment on _on_text_input()) are both normalized to a single LF,
            // so the rest of this field (line splitting, caret-by-line movement)
            // only ever has to deal with one kind of line break. Dropped outright
            // (same as any other control character) unless multiline is enabled.
            if (c == U'\r' || c == U'\n')
            {
                if (_multiline_enabled)
                    filtered += U'\n';
                continue;
            }

            if (c >= 0x20 && c != 0x7f)
                filtered += c;
        }

        if (filtered.empty())
            return;

        if (has_selection())
            _delete_selection();

        _text.insert(_caret_index, filtered);
        _caret_index += filtered.size();
        _selection_anchor = _caret_index;
        _on_text_changed();
    }

    /**
     * Consumes the horizontal arrows for the text caret (soft), so plain
     * arrow-key focus navigation yields while forced navigation (gamepad,
     * Alt+arrow) can still leave; same for the vertical arrows, but only
     * while get_multiline_enabled() (there's no vertical caret movement
     * otherwise, so they stay free for navigation, as before).
     */
    ArrowResponse TextField::_consumes_arrow(ArrowDirection dir) const
    {
        if (dir == ArrowDirection::Left || dir == ArrowDirection::Right)
            return ArrowResponse::Soft;
        if (_multiline_enabled && (dir == ArrowDirection::Up || dir == ArrowDirection::Down))
            return ArrowResponse::Soft;
        return ArrowResponse::Ignore;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns _Layout for _text, or empty if get_effective_style() has no font to
     * lay out against (yet).
     */
    std::optional<TextField::_Layout> TextField::_lay_out() const
    {
        Style style = get_effective_style();
        if (!style.font)
            return std::nullopt;

        const Padding padding = get_effective_padding();

        std::vector<cl7::text::codec::codepoint> codepoints = fonts::TextLayout::decode(_text);
        fonts::TextBlockLayout block = fonts::TextLayout::lay_out_box(codepoints, *style.font, style.text_style, {padding.left, padding.top - _scroll_offset_y}, _effective_text_box_size());

        return _Layout{std::move(style), std::move(codepoints), std::move(block)};
    }

    /**
     * Returns the index, within `line`, of the code point whose caret position
     * is closest to target_x (relative to the line's own start).
     */
    size_t TextField::_nearest_codepoint_in_line(const _Layout& layout, const fonts::TextLine& line, float target_x)
    {
        if (target_x <= 0.0f)
            return line.codepoint_begin;

        float previous_width = 0.0f;
        for (size_t i = line.codepoint_begin + 1; i <= line.codepoint_end; ++i)
        {
            const float width = fonts::TextLayout::measure_advance(std::span(layout.codepoints).subspan(line.codepoint_begin, i - line.codepoint_begin), *layout.style.font, layout.style.text_style);
            const float midpoint = (previous_width + width) * 0.5f;
            if (target_x < midpoint)
                return i - 1;
            previous_width = width;
        }

        return line.codepoint_end;
    }

    /**
     * Returns the index into `lines` of the line that `codepoint_index` falls
     * on: the last one whose own start still reaches it, same convention as
     * fonts::TextLayout::locate() (an index sitting exactly on a line break
     * lands on the following line, not the end of the one before it).
     */
    size_t TextField::_line_index_at(const std::vector<fonts::TextLine>& lines, size_t codepoint_index)
    {
        size_t line_index = 0;
        for (size_t i = 0; i < lines.size(); ++i)
            if (lines[i].codepoint_begin <= codepoint_index)
                line_index = i;
        return line_index;
    }

    /**
     * Returns the code point index within _text whose caret position is closest to
     * local_position (see _nearest_codepoint_in_line(), scoped to whichever line
     * local_position.y falls on).
     */
    size_t TextField::_codepoint_index_at(ml7::Vector2f local_position) const
    {
        const std::optional<_Layout> layout = _lay_out();
        if (!layout)
            return _text.size();

        // Clamps to the nearest actual line (above the first/below the last),
        // so a click above/below a multi-line field's text still lands
        // somewhere sensible, rather than nowhere at all.
        size_t line_index = 0;
        for (size_t i = 0; i < layout->block.lines.size(); ++i)
            if (layout->block.top + static_cast<float>(i) * layout->block.line_height <= local_position.y)
                line_index = i;

        return _nearest_codepoint_in_line(*layout, layout->block.lines[line_index], local_position.x - layout->block.left);
    }

    /**
     * Moves _caret_index to the nearest code point, roughly the same column
     * over, on the line `line_delta` lines away; a no-op if there's no such
     * line (already the first/last one).
     */
    void TextField::_move_caret_vertically(int line_delta)
    {
        const std::optional<_Layout> layout = _lay_out();
        if (!layout)
            return;

        const size_t current_line = _line_index_at(layout->block.lines, _caret_index);
        const ptrdiff_t target_line = static_cast<ptrdiff_t>(current_line) + line_delta;
        if (target_line < 0 || target_line >= static_cast<ptrdiff_t>(layout->block.lines.size()))
            return;

        const fonts::TextLine& from = layout->block.lines[current_line];
        const float current_x = fonts::TextLayout::measure_advance(std::span(layout->codepoints).subspan(from.codepoint_begin, _caret_index - from.codepoint_begin), *layout->style.font, layout->style.text_style);

        _caret_index = _nearest_codepoint_in_line(*layout, layout->block.lines[static_cast<size_t>(target_line)], current_x);
    }

    /**
     * Returns the start of the line `codepoint_index` falls on, or 0 if there's no
     * font to lay out against (yet).
     */
    size_t TextField::_line_start(size_t codepoint_index) const
    {
        const std::optional<_Layout> layout = _lay_out();
        if (!layout)
            return 0;

        return layout->block.lines[_line_index_at(layout->block.lines, codepoint_index)].codepoint_begin;
    }

    /**
     * Returns the end of the line `codepoint_index` falls on, or _text.size() if
     * there's no font to lay out against (yet).
     */
    size_t TextField::_line_end(size_t codepoint_index) const
    {
        const std::optional<_Layout> layout = _lay_out();
        if (!layout)
            return _text.size();

        return layout->block.lines[_line_index_at(layout->block.lines, codepoint_index)].codepoint_end;
    }

    /**
     * Returns this field's own size, narrowed by its own effective padding on
     * every side (see get_effective_padding()), and further by SCROLLBAR_THICKNESS
     * while get_multiline_enabled() and the scroll bar is actually shown (see
     * _relayout(), the only place that decides that); unchanged otherwise. This is
     * also what's reported via get_text_box_size(), so it's on this field itself
     * (rather than render::AbstractRenderer's own generic, padding-aware default)
     * to account for its own padding here.
     */
    ml7::Vector2f TextField::_effective_text_box_size() const
    {
        const Padding padding = get_effective_padding();
        ml7::Vector2f size = get_size();
        size.x = std::max(0.0f, size.x - padding.left - padding.right);
        size.y = std::max(0.0f, size.y - padding.top - padding.bottom);
        if (_multiline_enabled && _scroll_bar.is_visible())
            size.x = std::max(0.0f, size.x - SCROLLBAR_THICKNESS);
        return size;
    }

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
    void TextField::_relayout()
    {
        const ml7::Vector2f size = get_size();
        _scroll_bar.set_position({size.x - SCROLLBAR_THICKNESS, 0.0f});
        _scroll_bar.set_size({SCROLLBAR_THICKNESS, size.y});

        const Style style = get_effective_style();
        if (!_multiline_enabled || !style.font || _scroll_bar_visibility == ScrollBarVisibility::Never)
        {
            _scroll_bar.set_visible(false);
            _scroll_bar.set_enabled(false);
            _set_scroll_offset(0.0f);
            return;
        }

        const std::vector<cl7::text::codec::codepoint> codepoints = fonts::TextLayout::decode(_text);

        bool scroll_bar_shown = _scroll_bar_visibility == ScrollBarVisibility::Always;
        fonts::TextBlockLayout block;
        if (!scroll_bar_shown)
        {
            block = fonts::TextLayout::lay_out_box(codepoints, *style.font, style.text_style, {0.0f, 0.0f}, size);
            scroll_bar_shown = static_cast<float>(block.lines.size()) * block.line_height > size.y;
        }
        if (scroll_bar_shown)
        {
            const float width = std::max(0.0f, size.x - SCROLLBAR_THICKNESS);
            block = fonts::TextLayout::lay_out_box(codepoints, *style.font, style.text_style, {0.0f, 0.0f}, {width, size.y});
        }
        const float content_height = static_cast<float>(block.lines.size()) * block.line_height;

        _scroll_bar.set_visible(scroll_bar_shown);
        _scroll_bar.set_enabled(scroll_bar_shown);

        const float max_scroll = std::max(0.0f, content_height - size.y);
        _scroll_bar.set_range(0.0f, max_scroll);
        _scroll_bar.set_visible_fraction(content_height > 0.0f ? size.y / content_height : 1.0f);
        _scroll_bar.set_step(block.line_height > 0.0f ? block.line_height : 1.0f);

        _set_scroll_offset(_scroll_offset_y);
    }

    /**
     * Sets _scroll_offset_y, clamped to the scroll bar's own current range, and
     * syncs the scroll bar's own value to match.
     */
    void TextField::_set_scroll_offset(float scroll_offset_y)
    {
        _scroll_offset_y = std::clamp(scroll_offset_y, _scroll_bar.get_min(), _scroll_bar.get_max());
        _scroll_bar.set_value(_scroll_offset_y);
    }

    /**
     * Scrolls just enough (if at all) to bring the caret's own current line
     * back within view, while get_multiline_enabled(). A no-op otherwise, or
     * if it's already (at least partially) visible.
     */
    void TextField::_ensure_caret_visible()
    {
        if (!_multiline_enabled)
            return;

        const std::optional<_Layout> layout = _lay_out();
        if (!layout)
            return;

        const size_t line_index = _line_index_at(layout->block.lines, _caret_index);
        const float line_top = layout->block.top + static_cast<float>(line_index) * layout->block.line_height;
        const float line_bottom = line_top + layout->block.line_height;

        // line_top/line_bottom are already in this field's own viewport-local
        // space (the current scroll offset is baked into _lay_out()'s own box
        // position), so a negative/overflowing value directly says how far
        // off-screen (above/below) the caret's own line currently is.
        if (line_top < 0.0f)
            _set_scroll_offset(_scroll_offset_y + line_top);
        else if (line_bottom > get_size().y)
            _set_scroll_offset(_scroll_offset_y + (line_bottom - get_size().y));
    }

    /**
     * Runs _relayout() and _ensure_caret_visible(), then emits get_changed();
     * every place _text actually changes funnels through this, instead of
     * emitting directly.
     */
    void TextField::_on_text_changed()
    {
        _relayout();
        _ensure_caret_visible();
        _changed.emit(_text);
    }

    /**
     * Returns whether either Shift key is currently held.
     */
    bool TextField::_is_shift_down() const
    {
        const Shell* shell = get_shell();
        const xl7::input::Keyboard* keyboard = shell ? shell->get_keyboard() : nullptr;
        if (!keyboard)
            return false;

        return keyboard->is_key_down(xl7::input::Key::LeftShift) || keyboard->is_key_down(xl7::input::Key::RightShift);
    }

    /**
     * Returns whether either Control key is currently held.
     */
    bool TextField::_is_ctrl_down() const
    {
        const Shell* shell = get_shell();
        const xl7::input::Keyboard* keyboard = shell ? shell->get_keyboard() : nullptr;
        if (!keyboard)
            return false;

        return keyboard->is_key_down(xl7::input::Key::LeftControl) || keyboard->is_key_down(xl7::input::Key::RightControl);
    }

    /**
     * Returns the start of the word at/before index (skipping any whitespace right before it first).
     */
    size_t TextField::_previous_word_boundary(size_t index) const
    {
        while (index > 0 && cl7::text::inspect::is_whitespace(_text[index - 1]))
            --index;
        while (index > 0 && !cl7::text::inspect::is_whitespace(_text[index - 1]))
            --index;
        return index;
    }

    /**
     * Returns the start of the next word after index (skipping any whitespace right
     * after it first, then the rest of the current word, if any, then whatever
     * whitespace separates it from that next word), or _text.size().
     */
    size_t TextField::_next_word_boundary(size_t index) const
    {
        while (index < _text.size() && cl7::text::inspect::is_whitespace(_text[index]))
            ++index;
        while (index < _text.size() && !cl7::text::inspect::is_whitespace(_text[index]))
            ++index;
        while (index < _text.size() && cl7::text::inspect::is_whitespace(_text[index]))
            ++index;
        return index;
    }

    /**
     * Erases the current selection from _text, and collapses the caret (and
     * selection) to where it began.
     */
    void TextField::_delete_selection()
    {
        const size_t begin = get_selection_begin_codepoint_index();
        const size_t end = get_selection_end_codepoint_index();

        _text.erase(begin, end - begin);
        _caret_index = begin;
        _selection_anchor = begin;
    }



} // namespace fl7::gui::faces
