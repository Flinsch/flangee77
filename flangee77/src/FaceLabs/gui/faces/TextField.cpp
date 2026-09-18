#include "TextField.h"

#include "../Shell.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <XiaoLabs/Clipboard.h>

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // HasCaret Implementations
    // #############################################################################

    /**
     * Returns whether this text field is currently focused (the only time its caret is drawn).
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
     * Sets this text field's text, unless already that text; moves the caret to its end, and clears any selection.
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
     * Positions the caret at the clicked code point, and starts a fresh (empty) selection there.
     */
    void TextField::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position.x);
        _selection_anchor = _caret_index;
    }

    /**
     * Extends the selection from wherever the drag started to the code point now under the cursor.
     */
    void TextField::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position.x);
    }

    /**
     * Moves the caret, or deletes the code point before/after it (or, with an
     * active selection, deletes the whole selection instead). Shift+Left/Right/
     * Home/End extends the selection instead of moving/collapsing it. Ctrl+C/X/V
     * copies/cuts/pastes the selection via the system clipboard (see
     * xl7::Clipboard).
     */
    void TextField::_on_key_down(xl7::input::Key key)
    {
        using xl7::input::Key;

        const bool shift = _is_shift_down();
        const bool ctrl = _is_ctrl_down();

        switch (key)
        {
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
            else if (_caret_index > 0)
                --_caret_index;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::Right:
            if (!shift && has_selection())
                _caret_index = get_selection_end_codepoint_index();
            else if (_caret_index < _text.size())
                ++_caret_index;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::Home:
            _caret_index = 0;
            if (!shift)
                _selection_anchor = _caret_index;
            break;

        case Key::End:
            _caret_index = _text.size();
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
     * Inserts the input (filtered down to plain code points) at the caret, replacing any active selection.
     */
    void TextField::_on_text_input(const cl7::u32string& text)
    {
        cl7::u32string filtered;
        filtered.reserve(text.size());
        for (char32_t c : text)
            if (c >= 0x20 && c != 0x7f)
                filtered += c;

        if (filtered.empty())
            return;

        if (has_selection())
            _delete_selection();

        _text.insert(_caret_index, filtered);
        _caret_index += filtered.size();
        _selection_anchor = _caret_index;
        _changed.emit(_text);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the code point index within _text whose caret position is closest to local_x.
     */
    size_t TextField::_codepoint_index_at(float local_x) const
    {
        const Style style = get_effective_style();
        if (!style.font)
            return _text.size();

        const float target_x = local_x - style.text_style.background_padding.x;
        if (target_x <= 0.0f)
            return 0;

        float previous_width = 0.0f;
        for (size_t i = 1; i <= _text.size(); ++i)
        {
            const float width = fonts::TextLayout::measure_advance(cl7::u32string_view(_text).substr(0, i), *style.font, style.text_style);
            const float midpoint = (previous_width + width) * 0.5f;
            if (target_x < midpoint)
                return i - 1;
            previous_width = width;
        }

        return _text.size();
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
     * Erases the current selection from _text, and collapses the caret (and selection) to where it began.
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
