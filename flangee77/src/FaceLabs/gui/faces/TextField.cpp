#include "TextField.h"

#include "../Shell.h"
#include "../Style.h"

#include <FaceLabs/fonts/TextLayout.h>

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
     * Sets this text field's text, unless already that text; moves the caret to its end.
     */
    void TextField::set_text(cl7::u32string text)
    {
        if (_text == text)
            return;

        _text = std::move(text);
        _caret_index = _text.size();
        _changed.emit(_text);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Positions the caret at the clicked code point.
     */
    void TextField::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _caret_index = _codepoint_index_at(local_position.x);
    }

    /**
     * Moves the caret, or deletes the code point before/after it.
     */
    void TextField::_on_key_down(xl7::input::Key key)
    {
        using xl7::input::Key;

        switch (key)
        {
        case Key::Left:
            if (_caret_index > 0)
                --_caret_index;
            break;

        case Key::Right:
            if (_caret_index < _text.size())
                ++_caret_index;
            break;

        case Key::Home:
            _caret_index = 0;
            break;

        case Key::End:
            _caret_index = _text.size();
            break;

        case Key::Backspace:
            if (_caret_index > 0)
            {
                _text.erase(_caret_index - 1, 1);
                --_caret_index;
                _changed.emit(_text);
            }
            break;

        case Key::Delete:
            if (_caret_index < _text.size())
            {
                _text.erase(_caret_index, 1);
                _changed.emit(_text);
            }
            break;

        default:
            break;
        }
    }

    /**
     * Inserts the input (filtered down to plain code points) at the caret.
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

        _text.insert(_caret_index, filtered);
        _caret_index += filtered.size();
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



} // namespace fl7::gui::faces
