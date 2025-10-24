#ifndef FL7_FONTS_TEXTSTYLE_H
#define FL7_FONTS_TEXTSTYLE_H

#include <MathLabs/Vector2.h>
#include <XiaoLabs/graphics/Color.h>



namespace fl7::fonts {



/**
 *
 */
struct TextStyle
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

    /** The horizontal alignment (left, center, right, or justify). */
    HorizontalAlign horizontal_align = HorizontalAlign::Left;
    /** The vertical alignment (top, middle, bottom, or baseline). */
    VerticalAlign vertical_align = VerticalAlign::Baseline;

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
     * The "boldness" of the text in a normalized form, where 0 = normal, +1 = bold,
     * +2 = extra bold, -1 = thin, etc.
     */
    float weight = 0.0f;

    /**
     * The italic intensity, with 0 = normal, 1 = full italic, etc.
     * This is more of a pseudo-setting that does not really correspond to true
     * italics in the typographic sense. It's more of a geometric skew.
     */
    float italic_intensity = 0.0f;

    /** The actual (foreground) color of the text. */
    xl7::graphics::Color text_color = {1.0f, 1.0f, 1.0f, 1.0f};
    /** The optional background color (e.g., just transparent). */
    xl7::graphics::Color background_color = {0.0f, 0.0f, 0.0f, 0.0f};
    /** The color of the possible outline/stroke around text. */
    xl7::graphics::Color outline_color = {0.0f, 0.0f, 0.0f, 0.0f};
    /** The width of the outline/stroke around text (in pixels). */
    float outline_width = 0.0f;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_TEXTSTYLE_H
