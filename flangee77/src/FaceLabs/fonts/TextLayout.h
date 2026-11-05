#ifndef FL7_FONTS_TEXTLAYOUT_H
#define FL7_FONTS_TEXTLAYOUT_H

#include "./Font.h"
#include "./IconRun.h"
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

/**
 * Where a specific code point index ends up within a box-laid-out text (see
 * TextLayout::locate()): the absolute (top-left) position of a one-line-tall
 * caret/selection rect around it, plus that line's own height.
 */
struct CodepointPosition
{
    ml7::Vector2f position;
    float line_height = 0.0f;
};

/**
 * The result of laying `codepoints` out against a box (see
 * TextLayout::lay_out_box()): its lines, their shared height, and where the
 * first one actually starts: `top` at its own top edge (not its baseline),
 * `left` already includes the box's own horizontal padding, so a line's rect
 * is simply {left + measure_advance(line prefix), top + i * line_height} sized
 * {..., line_height}.
 */
struct TextBlockLayout
{
    std::vector<TextLine> lines;
    float line_height = 0.0f;
    float top = 0.0f;
    float left = 0.0f;
};

namespace TextLayout {

    /** Decodes `text` into a flat list of code points, for the span-based overloads below. */
    template <cl7::any_string_view_like Tstring_view_like>
    std::vector<cl7::text::codec::codepoint> decode(Tstring_view_like&& text)
    {
        auto sv = cl7::make_string_view(std::forward<Tstring_view_like>(text));
        using codepoint_iterator = cl7::text::codec::codepoint_iterator<cl7::char_type_of_t<Tstring_view_like>>;

        std::vector<cl7::text::codec::codepoint> codepoints;
        for (codepoint_iterator it(sv); it != codepoint_iterator(); ++it)
            codepoints.push_back(*it);
        return codepoints;
    }

    /**
     * Splits the given code points into lines.
     *
     * Explicit line breaks (CRLF, LF, or CR) always start a new line, regardless of
     * the wrap mode. If `text_style.wrap_mode` is `WrapMode::Word` and `max_width`
     * is positive, each such paragraph is additionally word-wrapped to fit within
     * `max_width` (greedily, breaking at whitespace; a single word wider than
     * `max_width` is hard-broken mid-word).
     *
     * A paragraph that ends up as a single, whole (non-word-wrapped) line has its
     * own leading/trailing whitespace trimmed from its range and width, same as
     * ordinary typeset display text (e.g., a Label) expects, unless
     * `text_style.preserve_whitespace` is set, in which case it's kept exactly as
     * authored instead, which an editable field's literal text (see TextField)
     * needs instead. A word-wrapped line's own boundaries never include the
     * whitespace that separated it from the next/previous word either, but that's
     * simply because those boundaries are word boundaries to begin with, regardless
     * of `preserve_whitespace`. A paragraph that's entirely whitespace under
     * `WrapMode::Word` likewise always collapses to an empty/blank line, consistent
     * with how word-wrap already normalizes the gaps between real words.
     * `text_style.letter_spacing` and `text_style.word_spacing` are incorporated
     * into each line's measured width and into wrap decisions.
     *
     * `icon_runs` (if any; must be sorted by `codepoint_index`) override the
     * advance width at their code point index with `icon->size.x` instead of
     * looking up a glyph there, and are never treated as whitespace for
     * word-splitting/justification purposes, regardless of the placeholder
     * code point actually present at that index.
     */
    std::vector<TextLine> lay_out(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style, float max_width, std::span<const IconRun> icon_runs = {});

    /**
     * Lays `codepoints` out exactly as AbstractRenderer::draw_text_in_box() would
     * against the same font/text_style/box_position/box_size, returning its lines
     * (see lay_out()) plus where (and how tall) each one actually starts, so a
     * caller can turn any of them into on-screen rects without reimplementing the
     * box/wrap/vertical-alignment math itself (see locate(), and
     * gui::render::SelectionHelper for a multi-line selection highlight, one rect
     * per spanned line). Horizontal alignment always assumes
     * TextStyle::HorizontalAlign::Left (Center/Right/Justify are not accounted
     * for); vertical alignment (Top/Middle/Bottom/Baseline) is honored exactly.
     */
    TextBlockLayout lay_out_box(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style, ml7::Vector2f box_position, ml7::Vector2f box_size);

    /** Same as the `codepoints`-based overload, but decodes `text` first. */
    template <cl7::any_string_view_like Tstring_view_like>
    TextBlockLayout lay_out_box(Tstring_view_like&& text, Font& font, const TextStyle& text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
    {
        return lay_out_box(decode(std::forward<Tstring_view_like>(text)), font, text_style, box_position, box_size);
    }

    /**
     * Locates where the code point at `codepoint_index` (clamped to
     * `codepoints.size()`) would end up if `codepoints` were drawn via
     * AbstractRenderer::draw_text_in_box() against the same font/text_style/
     * box_position/box_size (see lay_out_box()): the line it falls on and the
     * horizontal advance into that line, combined into a single absolute
     * position (the top-left corner of a one-line-tall rect around it), plus
     * that line's own height. For an index sitting exactly on a line break
     * (word-wrap or explicit), this is the START of the following line, not
     * the end of the one before it, same as a caret naturally continues there.
     *
     * Meant for gui::render::CaretHelper to position a caret rect against text
     * that may since have grown to more than one line (see
     * gui::faces::TextField's own multiline support).
     */
    CodepointPosition locate(std::span<const cl7::text::codec::codepoint> codepoints, size_t codepoint_index, Font& font, const TextStyle& text_style, ml7::Vector2f box_position, ml7::Vector2f box_size);

    /** Same as the `codepoints`-based overload, but decodes `text` first. */
    template <cl7::any_string_view_like Tstring_view_like>
    CodepointPosition locate(Tstring_view_like&& text, size_t codepoint_index, Font& font, const TextStyle& text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
    {
        return locate(decode(std::forward<Tstring_view_like>(text)), codepoint_index, font, text_style, box_position, box_size);
    }

    /**
     * Measures the raw pen-advance width (in scaled pixels) that `codepoints`
     * would occupy on a single, unwrapped line under `text_style` and `font`,
     * exactly as `lay_out()` would actually draw it (`WrapMode::None`): the
     * plain left-to-right sum of each code point's advance plus
     * `text_style.letter_spacing`/`word_spacing`, with whitespace (leading,
     * trailing, or in between) included verbatim. Unlike `measure_width()`
     * above, this is never trimmed. Meant for a caret/cursor's pixel position
     * (see `gui::render::CaretHelper`): call this with the prefix of the same
     * code points up to the caret's code point index.
     */
    float measure_advance(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style);

    /** Same as the `codepoints`-based overload, but decodes `text` first. */
    template <cl7::any_string_view_like Tstring_view_like>
    float measure_advance(Tstring_view_like&& text, Font& font, const TextStyle& text_style)
    {
        return measure_advance(decode(std::forward<Tstring_view_like>(text)), font, text_style);
    }

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
        const std::vector<cl7::text::codec::codepoint> codepoints = decode(std::forward<Tstring_view_like>(text));

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
