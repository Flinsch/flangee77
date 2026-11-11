#include "Button.h"

#include <FaceLabs/fonts/TextLayout.h>

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Returns this button's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f Button::_calculate_preferred_size() const
    {
        return _calculate_text_size();
    }

    /**
     * Returns this button's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f Button::_calculate_minimum_size() const
    {
        return _calculate_text_size();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns this button's text's own natural (unwrapped) size: however wide its
     * single widest (explicit-break-delimited) line is, times however many such
     * lines there are, under its effective font/text style. {0, 0} if it currently
     * has no effective font (e.g., not yet part of a themed Shell). Cached, as
     * long as neither the text nor the effective font have changed since.
     */
    ml7::Vector2f Button::_calculate_text_size() const
    {
        const Style style = get_effective_style();
        if (!style.font)
            return {};

        if (style.font == _cached_font && _text == _cached_text)
            return _cached_text_size;

        fonts::TextStyle unwrapped_style = style.text_style;
        unwrapped_style.wrap_mode = fonts::TextStyle::WrapMode::None;

        const auto codepoints = fonts::TextLayout::decode(_text);
        const fonts::TextBlockLayout block = fonts::TextLayout::lay_out_box(codepoints, *style.font, unwrapped_style, {0.0f, 0.0f}, {0.0f, 0.0f});

        float width = 0.0f;
        for (const fonts::TextLine& line : block.lines)
            width = std::max(width, line.width);

        _cached_font = style.font;
        _cached_text = _text;
        _cached_text_size = {width, static_cast<float>(block.lines.size()) * block.line_height};

        return _cached_text_size;
    }



} // namespace fl7::gui::faces
