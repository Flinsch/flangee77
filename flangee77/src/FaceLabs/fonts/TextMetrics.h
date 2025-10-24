#ifndef FL7_FONTS_TEXTMETRICS_H
#define FL7_FONTS_TEXTMETRICS_H

#include "./Font.h"
#include "./FontMetrics.h"
#include "./TextStyle.h"

#include <CoreLabs/text/codec/codepoint.h>

#include <vector>



namespace fl7::fonts {



/**
 * Calculates the size of specific texts based on font metrics and text styles.
 */
struct TextMetrics
{
    /** The font size (in pixels), scaled horizontally and vertically. */
    ml7::Vector2f scaled_font_size;

    TextMetrics() = default;
    TextMetrics(const std::vector<cl7::text::codec::codepoint>& codepoints, Font& font, const TextStyle& text_style);
};



} // namespace fl7::fonts

#endif // FL7_FONTS_TEXTMETRICS_H
