#include "AbstractRenderer.h"



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



    void AbstractRenderer::_draw_codepoints(const std::vector<cl7::text::codec::codepoint>& codepoints, Font* font, const TextStyle* text_style)
    {
        if (!font) return;
        if (!text_style) text_style = &_default_text_style;

        Font::Access font_access = font->access();
        FontMetrics font_metrics = font_access.get_metrics();

        State state = {
            .font_access = std::move(font_access),
            .font_metrics = font_metrics,
            .text_style = *text_style,
            .text_metrics = TextMetrics{codepoints, *font, *text_style},
            .cursor = {0.0f, 0.0f},
        };

        ScopedBatch auto_batch(_batch_depth > 0 ? nullptr : this);

        for (auto codepoint : codepoints)
        {
            _emit_codepoint(codepoint, state);
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
