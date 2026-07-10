#include "TextLayout.h"

#include <CoreLabs/text/inspect.h>



namespace fl7::fonts {



namespace {

    bool _is_whitespace(cl7::text::codec::codepoint cp)
    {
        return cl7::text::inspect::is_whitespace(static_cast<char32_t>(cp.value));
    }

    size_t _line_break_length(cl7::text::codec::codepoint c0, cl7::text::codec::codepoint c1)
    {
        return cl7::text::inspect::is_line_break_relaxed(static_cast<char32_t>(c0.value), static_cast<char32_t>(c1.value));
    }

    float _advance_of(cl7::text::codec::codepoint cp, Font::Access& font_access, ml7::Vector2f scaled_font_size)
    {
        const Glyph* glyph = font_access.find_glyph(cp);
        if (!glyph)
            return 0.0f;
        return glyph->metrics.advance_width * scaled_font_size.x;
    }

    float _measure(std::span<const cl7::text::codec::codepoint> codepoints, size_t begin, size_t end, Font::Access& font_access, ml7::Vector2f scaled_font_size)
    {
        float width = 0.0f;
        for (size_t i = begin; i < end; ++i)
            width += _advance_of(codepoints[i], font_access, scaled_font_size);
        return width;
    }

    /** A whitespace-delimited word within a paragraph, as a code point range. */
    struct _Word
    {
        size_t begin;
        size_t end;
        float width;
    };

    std::vector<_Word> _split_into_words(std::span<const cl7::text::codec::codepoint> codepoints, size_t begin, size_t end, Font::Access& font_access, ml7::Vector2f scaled_font_size)
    {
        std::vector<_Word> words;

        size_t i = begin;
        while (i < end)
        {
            while (i < end && _is_whitespace(codepoints[i]))
                ++i;
            if (i >= end)
                break;

            const size_t word_begin = i;
            while (i < end && !_is_whitespace(codepoints[i]))
                ++i;
            const size_t word_end = i;

            words.push_back(_Word{
                .begin = word_begin,
                .end = word_end,
                .width = _measure(codepoints, word_begin, word_end, font_access, scaled_font_size),
            });
        }

        return words;
    }

    TextLine _make_trimmed_line(std::span<const cl7::text::codec::codepoint> codepoints, size_t begin, size_t end, Font::Access& font_access, ml7::Vector2f scaled_font_size)
    {
        while (begin < end && _is_whitespace(codepoints[begin]))
            ++begin;
        while (end > begin && _is_whitespace(codepoints[end - 1]))
            --end;

        // A trimmed line is always its paragraph's only (and thus last) line.
        return TextLine{
            .codepoint_begin = begin,
            .codepoint_end = end,
            .width = _measure(codepoints, begin, end, font_access, scaled_font_size),
            .word_count = static_cast<unsigned>(_split_into_words(codepoints, begin, end, font_access, scaled_font_size).size()),
            .is_paragraph_end = true,
        };
    }

    /**
     * Hard-breaks a single (too-wide-for-any-line) word into as many lines as
     * needed, each holding as many codepoints as fit within `max_width` (at least
     * one, to guarantee progress).
     */
    void _hard_break_word(std::span<const cl7::text::codec::codepoint> codepoints, size_t word_begin, size_t word_end, Font::Access& font_access, ml7::Vector2f scaled_font_size, float max_width, std::vector<TextLine>& out_lines)
    {
        size_t piece_begin = word_begin;
        float piece_width = 0.0f;

        for (size_t i = word_begin; i < word_end; ++i)
        {
            const float advance = _advance_of(codepoints[i], font_access, scaled_font_size);
            if (piece_width + advance > max_width && i > piece_begin)
            {
                // A fragment of a single word has no internal gaps, so it can
                // never be justified (word_count == 1).
                out_lines.push_back(TextLine{.codepoint_begin = piece_begin, .codepoint_end = i, .width = piece_width, .word_count = 1});
                piece_begin = i;
                piece_width = 0.0f;
            }
            piece_width += advance;
        }

        out_lines.push_back(TextLine{.codepoint_begin = piece_begin, .codepoint_end = word_end, .width = piece_width, .word_count = 1});
    }

    void _word_wrap_paragraph(std::span<const cl7::text::codec::codepoint> codepoints, size_t begin, size_t end, Font::Access& font_access, ml7::Vector2f scaled_font_size, float max_width, std::vector<TextLine>& out_lines)
    {
        const std::vector<_Word> words = _split_into_words(codepoints, begin, end, font_access, scaled_font_size);
        if (words.empty())
        {
            out_lines.push_back(_make_trimmed_line(codepoints, begin, end, font_access, scaled_font_size));
            return;
        }

        size_t k = 0;
        while (k < words.size())
        {
            if (words[k].width > max_width)
            {
                // A single word that doesn't fit on any line, regardless: break it up.
                _hard_break_word(codepoints, words[k].begin, words[k].end, font_access, scaled_font_size, max_width, out_lines);
                ++k;
                continue;
            }

            const size_t line_start_word = k;
            size_t line_end_cp = words[k].end;
            ++k;

            while (k < words.size())
            {
                const float tentative_width = _measure(codepoints, words[line_start_word].begin, words[k].end, font_access, scaled_font_size);
                if (tentative_width > max_width)
                    break;
                line_end_cp = words[k].end;
                ++k;
            }

            out_lines.push_back(TextLine{
                .codepoint_begin = words[line_start_word].begin,
                .codepoint_end = line_end_cp,
                .width = _measure(codepoints, words[line_start_word].begin, line_end_cp, font_access, scaled_font_size),
                .word_count = static_cast<unsigned>(k - line_start_word),
            });
        }

        // Whichever line was pushed last for this paragraph (whether from the
        // word-fitting branch above or a hard break) is its last line.
        out_lines.back().is_paragraph_end = true;
    }

} // namespace



namespace TextLayout {

    /**
     * Splits the given codepoints into lines.
     *
     * Explicit line breaks (CRLF, LF, or CR) always start a new line, regardless of
     * the wrap mode. If `text_style.wrap_mode` is `WrapMode::Word` and `max_width`
     * is positive, each such paragraph is additionally word-wrapped to fit within
     * `max_width` (greedily, breaking at whitespace; a single word wider than
     * `max_width` is hard-broken mid-word). Leading/trailing whitespace of each
     * produced line is excluded from its range and width.
     */
    std::vector<TextLine> lay_out(std::span<const cl7::text::codec::codepoint> codepoints, Font& font, const TextStyle& text_style, float max_width)
    {
        Font::Access font_access = font.access();
        const ml7::Vector2f scaled_font_size = text_style.scaling * text_style.font_size;

        // Phase 1: always split on explicit line breaks into paragraphs.
        std::vector<std::pair<size_t, size_t>> paragraphs;
        size_t paragraph_begin = 0;
        size_t i = 0;
        while (i < codepoints.size())
        {
            const cl7::text::codec::codepoint c0 = codepoints[i];
            const cl7::text::codec::codepoint c1 = (i + 1 < codepoints.size()) ? codepoints[i + 1] : cl7::text::codec::codepoint{};
            const size_t break_length = _line_break_length(c0, c1);
            if (break_length > 0)
            {
                paragraphs.emplace_back(paragraph_begin, i);
                i += break_length;
                paragraph_begin = i;
            }
            else
            {
                ++i;
            }
        }
        paragraphs.emplace_back(paragraph_begin, codepoints.size());

        // Phase 2: per paragraph, either keep it as a single line or word-wrap it.
        const bool word_wrap = text_style.wrap_mode == TextStyle::WrapMode::Word && max_width > 0.0f;

        std::vector<TextLine> lines;
        lines.reserve(paragraphs.size());
        for (const auto& [begin, end] : paragraphs)
        {
            if (!word_wrap || begin == end)
                lines.push_back(_make_trimmed_line(codepoints, begin, end, font_access, scaled_font_size));
            else
                _word_wrap_paragraph(codepoints, begin, end, font_access, scaled_font_size, max_width, lines);
        }

        return lines;
    }

} // namespace TextLayout



} // namespace fl7::fonts
