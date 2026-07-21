#ifndef FL7_FONTS_TEXTLAYOUT_H
#define FL7_FONTS_TEXTLAYOUT_H

#include "./Font.h"
#include "./TextStyle.h"

#include <CoreLabs/string.h>
#include <CoreLabs/text/codec/codepoint.h>
#include <CoreLabs/text/codec/codepoint_iterator.h>

#include <algorithm>
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
     * produced line is excluded from its range and width. `text_style.letter_spacing`
     * and `text_style.word_spacing` are incorporated into each line's measured
     * width and into wrap decisions.
     */
    std::vector<TextLine> lay_out(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style, float max_width);

    /**
     * Measures the width (in scaled pixels) that `text` would occupy on a single,
     * unwrapped line under `text_style` and `font`, i.e., as if laid out with
     * `WrapMode::None` (`text_style.wrap_mode` is ignored). Useful for positioning
     * things relative to text without actually drawing it (e.g., placing a second
     * block of content to the right of some text). Explicit line breaks still start
     * new lines, in which case the width of the widest one is returned.
     */
    template <cl7::any_string_view_like Tstring_view_like>
    float measure_width(Tstring_view_like&& text, Font& font, const TextStyle& text_style)
    {
        auto sv = cl7::make_string_view(std::forward<Tstring_view_like>(text));
        using codepoint_iterator = cl7::text::codec::codepoint_iterator<cl7::char_type_of_t<Tstring_view_like>>;

        std::vector<cl7::text::codec::codepoint> codepoints;
        for (codepoint_iterator it(sv); it != codepoint_iterator(); ++it)
            codepoints.push_back(*it);

        TextStyle unwrapped_style = text_style;
        unwrapped_style.wrap_mode = TextStyle::WrapMode::None;

        float max_line_width = 0.0f;
        for (const TextLine& line : lay_out(codepoints, font, unwrapped_style, 0.0f))
            max_line_width = std::max(max_line_width, line.width);
        return max_line_width;
    }

} // namespace TextLayout



} // namespace fl7::fonts

#endif // FL7_FONTS_TEXTLAYOUT_H
