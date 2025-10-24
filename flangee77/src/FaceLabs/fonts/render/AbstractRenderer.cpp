#include "AbstractRenderer.h"



namespace fl7::fonts::render {



    /**
     * Begins a new text rendering batch. Must be balled before issuing any draw
     * commands if batching manually. Typically followed by multiple calls to
     * `draw_text`, and then a final call to `end`. If you only call `draw_text`
     * occasionally, it is not necessary to call `begin` or `end` manually; the
     * renderer will manage batching internally.
     */
    void AbstractRenderer::begin()
    {
        if (_is_batching)
            return;

        _before_begin();
        _is_batching = true;
    }

    /**
     * Submits any queued geometry and finalizes the current text rendering batch.
     * This implicitly calls `flush`.
     */
    void AbstractRenderer::end()
    {
        if (!_is_batching)
            return;

        flush();

        _is_batching = false;
        _after_end();
    }

    /**
     * Forces the submission of all currently batched geometry. This can be called
     * manually if external render state changes, or when mixing with other
     * rendering systems that may modify render state.
     */
    void AbstractRenderer::flush()
    {
        if (!_is_batching)
            return;

        _do_flush();
    }



    void AbstractRenderer::_draw_codepoints(const std::vector<cl7::text::codec::codepoint>& codepoints, Font* font, const TextStyle* text_style)
    {
        if (!font) return;
        if (!text_style) text_style = &_default_text_style;

        State state = {
            .font = font,
            .font_metrics = &font->get_metrics(),
            .text_style = text_style,
            .text_metrics = TextMetrics{codepoints, *font, *text_style},
            .cursor = {0.0f, 0.0f},
        };

        AutoBeginEnd auto_begin_end(_is_batching ? nullptr : this);

        for (auto codepoint : codepoints)
        {
            _emit_codepoint(codepoint, state);
        }
    }

    void AbstractRenderer::_emit_codepoint(cl7::text::codec::codepoint codepoint, State& state)
    {
        const Glyph* glyph = state.font->find_glyph(codepoint);
        if (!glyph)
            return;

        _emit_glyph(*glyph, state);

        state.cursor.x += glyph->advance_width * state.text_metrics.scaled_font_size.x;
    }



} // namespace fl7::fonts::render
