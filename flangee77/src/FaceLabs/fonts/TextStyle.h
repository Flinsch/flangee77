#ifndef FL7_FONTS_TEXTSTYLE_H
#define FL7_FONTS_TEXTSTYLE_H

#include "./GlyphStyle.h"

#include <MathLabs/Vector2.h>
#include <XiaoLabs/graphics/Color.h>



namespace fl7::fonts {



/**
 *
 */
struct TextStyle
    : GlyphStyle
{
    enum struct HorizontalAlign
    {
        Left,
        Center,
        Right,
        Justify,
    };

    enum struct VerticalAlign
    {
        Top,
        Middle,
        Bottom,
        Baseline,
    };

    enum struct WrapMode
    {
        /** No wrapping: a line only ends at an explicit line break (if any). */
        None,
        /** Greedy word-wrap at whitespace, breaking mid-word only if a single word doesn't fit. */
        Word,
    };

    /** The horizontal alignment (left, center, right, or justify). */
    HorizontalAlign horizontal_align = HorizontalAlign::Left;
    /** The vertical alignment (top, middle, bottom, or baseline). */
    VerticalAlign vertical_align = VerticalAlign::Baseline;
    /** The wrapping behavior for multi-line (box-based) text. */
    WrapMode wrap_mode = WrapMode::None;

    /** The base font size, in pixels. */
    float font_size = 16.0f;
    /** The relative horizontal and vertical scaling factors. */
    ml7::Vector2f scaling = {1.0f, 1.0f};

    /** The relative line spacing (1.0 = normal). */
    float line_spacing = 1.0f;
    /** The additional letter spacing between characters (in pixels). */
    float letter_spacing = 0.0f;
    /** The additional spacing between words (in pixels). */
    float word_spacing = 0.0f;

    /**
     * Extra horizontal (x) and vertical (y) space added around each line's
     * background, on top of its natural (bearing-based/line-height) extent. For
     * box-based text, the horizontal component also reserves space at the box's
     * left/right edges (reducing the effective wrap width and inset alignment
     * accordingly), so the padded background never overflows the box. The
     * vertical component is purely visual: it only grows the background rect,
     * it never affects line positioning.
     */
    ml7::Vector2f background_padding = {0.0f, 0.0f};

    /** The optional background color (e.g., just transparent). */
    xl7::graphics::Color background_color = {0.0f, 0.0f, 0.0f, 0.0f};
};



} // namespace fl7::fonts

#endif // FL7_FONTS_TEXTSTYLE_H
