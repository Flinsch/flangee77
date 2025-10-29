#ifndef FL7_FONTS_FONTMETRICS_H
#define FL7_FONTS_FONTMETRICS_H

#include <MathLabs/Vector2.h>



namespace fl7::fonts {



/**
 * Provides general dimensions of glyphs and other typographic properties of a font.
 * All values are in em and relative to the implicit baseline of 0.
 */
struct FontMetrics
{
    /** The typographic ascent of the font. */
    float ascent = 0.0f;
    /** The typographic descent of the font. */
    float descent = 0.0f;
    /** The typographic leading (line gap) of the font. */
    float leading = 0.0f;
    /** The line height of the font (ascent + descent + leading). */
    float line_height = 0.0f;
    /** The cap height: the height of a capital letter above the baseline (a flat letter such as 'H' or 'I'; not round or pointed letters like 'O' or 'A', which may overshoot). */
    float cap_height = 0.0f;
    /** The x-height (corpus size): the height of the main body of the lowercase letter 'x' in the font, which measures from the baseline up to the meanline/median. */
    float x_height = 0.0f;

    /** The minimum left-side bearing for (non-empty) glyphs in the font. */
    float min_left_bearing = 0.0f;
    /** The minimum right-side bearing for (non-empty) glyphs in the font. */
    float min_right_bearing = 0.0f;
    /** The maximum width of the widest glyph in the font. */
    float max_width = 0.0f;
    /** The maximum advance width of glyphs in the font. */
    float max_advance_width = 0.0f;
    /** The average (advance) width of non-empty glyphs in the font. */
    float average_width = 0.0f;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_FONTMETRICS_H
