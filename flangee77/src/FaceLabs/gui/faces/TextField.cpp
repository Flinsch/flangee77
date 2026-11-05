#include "TextField.h"

#include "../Shell.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <XiaoLabs/Clipboard.h>

#include <CoreLabs/text/inspect.h>

#include <algorithm>
#include <span>



namespace fl7::gui::faces {



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
        _changed.emit(_text);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Positions the caret at the clicked code point, and starts a fresh (empty)
     * selection there.
     */
    void TextField::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position);
        _selection_anchor = _caret_index;
    }

    /**
     * Extends the selection from wherever the drag started to the code point now
     * under the cursor.
     */
    void TextField::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position);
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
                _changed.emit(_text);
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
                _changed.emit(_text);
            }
            else if (_caret_index > 0)
            {
                _text.erase(_caret_index - 1, 1);
                --_caret_index;
                _selection_anchor = _caret_index;
                _changed.emit(_text);
            }
            break;

        case Key::Delete:
            if (has_selection())
            {
                _delete_selection();
                _changed.emit(_text);
            }
            else if (_caret_index < _text.size())
            {
                _text.erase(_caret_index, 1);
                _selection_anchor = _caret_index;
                _changed.emit(_text);
            }
            break;

        default:
            break;
        }
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
        _changed.emit(_text);
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

        std::vector<cl7::text::codec::codepoint> codepoints = fonts::TextLayout::decode(_text);
        fonts::TextBlockLayout block = fonts::TextLayout::lay_out_box(codepoints, *style.font, style.text_style, {0.0f, 0.0f}, get_size());

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
