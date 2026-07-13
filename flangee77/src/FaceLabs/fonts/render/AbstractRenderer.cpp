#include "AbstractRenderer.h"

#include "../TextLayout.h"

#include <CoreLabs/text/inspect.h>



namespace fl7::fonts::render {



    /**
     * Creates a new `ScopedBatch` object to manage a text rendering batch.
     *
     * Use this function to start a new batch. The returned `ScopedBatch` object
     * will automatically call `end` when it goes out of scope, unless `end` is
     * called explicitly beforehand.
     */
    AbstractRenderer::ScopedBatch AbstractRenderer::begin_batch()
    {
        return ScopedBatch{this};
    }



    /**
     * Begins a new text rendering batch. Must be balled before issuing any draw
     * commands if batching manually. Typically followed by multiple calls to
     * `draw_text`, and then a final call to `end`. If you only call `draw_text`
     * occasionally, it is not necessary to call `begin` or `end` manually; the
     * renderer will manage batching internally.
     *
     * For greater convenience, the RAII helper `ScopedBatch` should be used via
     * `begin_batch`.
     */
    void AbstractRenderer::begin()
    {
        if (_batch_depth++ == 0)
            _before_begin();
    }

    /**
     * Submits any queued geometry and finalizes the current text rendering batch.
     * This implicitly calls `flush`.
     *
     * For greater convenience, the RAII helper `ScopedBatch` should be used via
     * `begin_batch`, which ultimately calls `end` automatically.
     */
    void AbstractRenderer::end()
    {
        assert(_batch_depth > 0);
        flush();

        if (_batch_depth > 0 && --_batch_depth == 0)
            _after_end();
    }

    /**
     * Forces the submission of all currently batched geometry. This can be called
     * manually if external render state changes, or when mixing with other
     * rendering systems that may modify render state.
     */
    void AbstractRenderer::flush()
    {
        assert(_batch_depth > 0);
        _do_flush();
    }



    void AbstractRenderer::_draw_codepoints_in_box(const std::vector<cl7::text::codec::codepoint>& codepoints, Font* font, const TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
    {
        if (!font) return;
        if (!text_style) text_style = &_default_text_style;

        Font::Access font_access = font->access();
        FontMetrics font_metrics = font_access.get_metrics();
        TextMetrics text_metrics{codepoints, *font, *text_style};

        // Word-wrapping only makes sense against a real box width: a zero-size
        // box (used internally for point-based `draw_text`) only ever splits on
        // explicit line breaks.
        TextStyle wrap_text_style = *text_style;
        if (box_size.x <= 0.0f)
            wrap_text_style.wrap_mode = TextStyle::WrapMode::None;

        const std::vector<TextLine> lines = TextLayout::lay_out(codepoints, *font, wrap_text_style, box_size.x);

        const float line_height_px = font_metrics.line_height * text_metrics.scaled_font_size.y * text_style->line_spacing;
        const float block_height = static_cast<float>(lines.size()) * line_height_px;
        const float ascent_px = font_metrics.ascent * text_metrics.scaled_font_size.y;

        float line_y;
        switch (text_style->vertical_align)
        {
        case TextStyle::VerticalAlign::Top:
            line_y = box_position.y + ascent_px;
            break;
        case TextStyle::VerticalAlign::Middle:
            line_y = box_position.y + (box_size.y - block_height) * 0.5f + ascent_px;
            break;
        case TextStyle::VerticalAlign::Bottom:
            line_y = box_position.y + box_size.y - block_height + ascent_px;
            break;
        case TextStyle::VerticalAlign::Baseline:
        default:
            line_y = box_position.y;
            break;
        }

        State state = {
            .font_access = std::move(font_access),
            .font_metrics = font_metrics,
            .text_style = *text_style,
            .text_metrics = text_metrics,
            .cursor = {},
            .box_position = box_position,
            .box_size = box_size,
        };

        ScopedBatch auto_batch(_batch_depth > 0 ? nullptr : this);

        for (const TextLine& line : lines)
        {
            // Justify stretches the gaps between words to fill the box width, but
            // only where that's meaningful: not on a paragraph's last (or only)
            // line, and not on a line with just one word (no gaps to stretch).
            // Both fall back to Left, per standard convention.
            const bool is_justified = text_style->horizontal_align == TextStyle::HorizontalAlign::Justify
                && box_size.x > 0.0f && !line.is_paragraph_end && line.word_count > 1;

            float start_x;
            switch (text_style->horizontal_align)
            {
            case TextStyle::HorizontalAlign::Center:
                start_x = box_position.x + (box_size.x - line.width) * 0.5f;
                break;
            case TextStyle::HorizontalAlign::Right:
                start_x = box_position.x + box_size.x - line.width;
                break;
            case TextStyle::HorizontalAlign::Left:
            case TextStyle::HorizontalAlign::Justify:
            default:
                start_x = box_position.x;
                break;
            }

            const float extra_per_gap = is_justified ? (box_size.x - line.width) / static_cast<float>(line.word_count - 1) : 0.0f;

            state.cursor = {start_x, line_y};
            bool previous_was_whitespace = false;
            for (size_t i = line.codepoint_begin; i < line.codepoint_end; ++i)
            {
                const cl7::text::codec::codepoint codepoint = codepoints[i];

                const bool is_whitespace = cl7::text::inspect::is_whitespace(static_cast<char32_t>(codepoint.value));
                if (is_whitespace && !previous_was_whitespace)
                    state.cursor.x += extra_per_gap + text_style->word_spacing;
                previous_was_whitespace = is_whitespace;

                _emit_codepoint(codepoint, state);

                if (i + 1 < line.codepoint_end)
                    state.cursor.x += text_style->letter_spacing;
            }

            line_y += line_height_px;
        }
    }

    void AbstractRenderer::_emit_codepoint(cl7::text::codec::codepoint codepoint, State& state)
    {
        const Glyph* glyph = state.font_access.find_glyph(codepoint);
        if (!glyph)
            return;

        _emit_glyph(*glyph, state);

        state.cursor.x += glyph->metrics.advance_width * state.text_metrics.scaled_font_size.x;
    }



} // namespace fl7::fonts::render
