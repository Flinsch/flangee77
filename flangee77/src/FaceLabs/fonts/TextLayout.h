#ifndef FL7_FONTS_TEXTLAYOUT_H
#define FL7_FONTS_TEXTLAYOUT_H

#include "./Font.h"
#include "./TextStyle.h"

#include <CoreLabs/text/codec/codepoint.h>

#include <span>
#include <vector>



namespace fl7::fonts {



/**
 * Describes a single laid-out line of text as a range into the caller's
 * codepoint sequence, along with its measured (scaled) width.
 */
struct TextLine
{
    /** The index of the first code point of this line (inclusive). */
    size_t codepoint_begin = 0;
    /** The index of the first code point after this line (exclusive). */
    size_t codepoint_end = 0;
    /** The measured width of this line, in scaled pixels. */
    float width = 0.0f;
    /** The number of whitespace-delimited words on this line. */
    unsigned word_count = 0;
    /** Whether this is the last (or only) line of its paragraph. */
    bool is_paragraph_end = false;
};

namespace TextLayout {

    /**
     * Splits the given code points into lines.
     *
     * Explicit line breaks (CRLF, LF, or CR) always start a new line, regardless of
     * the wrap mode. If `text_style.wrap_mode` is `WrapMode::Word` and `max_width`
     * is positive, each such paragraph is additionally word-wrapped to fit within
     * `max_width` (greedily, breaking at whitespace; a single word wider than
     * `max_width` is hard-broken mid-word). Leading/trailing whitespace of each
     * produced line is excluded from its range and width.
     */
    std::vector<TextLine> lay_out(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style, float max_width);

} // namespace TextLayout



} // namespace fl7::fonts

#endif // FL7_FONTS_TEXTLAYOUT_H
