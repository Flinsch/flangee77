#ifndef FL7_FONTS_GLYPHSTYLE_H
#define FL7_FONTS_GLYPHSTYLE_H

#include <XiaoLabs/graphics/Color.h>



namespace fl7::fonts {



/**
 * The subset of TextStyle that applies per glyph rather than per line/box,
 * and is therefore the part a StyleRun can override for a sub-range of a
 * single draw call. TextStyle inherits this for its own (default) values.
 */
struct GlyphStyle
{
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
    /** The color of the possible outline/stroke around text. */
    xl7::graphics::Color outline_color = {0.0f, 0.0f, 0.0f, 0.0f};
    /** The width of the outline/stroke around text (in pixels). */
    float outline_width = 0.0f;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_GLYPHSTYLE_H
