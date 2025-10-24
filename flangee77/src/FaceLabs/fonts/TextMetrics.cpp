#include "TextMetrics.h"



namespace fl7::fonts {



    TextMetrics::TextMetrics(const std::vector<cl7::text::codec::codepoint>& codepoints, Font& font, const TextStyle& text_style)
    {
        scaled_font_size = text_style.scaling * text_style.font_size;
    }



} // namespace fl7::fonts
