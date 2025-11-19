#ifndef FL7_FONTS_RENDER_ABSTRACTRENDERER_H
#define FL7_FONTS_RENDER_ABSTRACTRENDERER_H

#include "../Font.h"
#include "../FontMetrics.h"
#include "../TextMetrics.h"
#include "../TextStyle.h"

#include <CoreLabs/string.h>

#include "CoreLabs/text/codec/codepoint_iterator.h"



namespace fl7::fonts::render {



/**
 * Abstract class for rendering text.
 */
class AbstractRenderer
{

public:
    /**
     * A RAII helper for managing text rendering batches in a scoped and nestable way.
     *
     * This class automates the process of beginning and ending a text rendering
     * batch. It is designed to be used in a scoped manner (e.g., within a block or
     * function), ensuring that `end` is called automatically when the `ScopedBatch`
     * object goes out of scope.
     *
     * Supports nesting: multiple `ScopedBatch` objects can be active simultaneously
     * on the same renderer, but only the outermost `begin`/`end` calls affect the
     * actual renderer state.
     */
    class ScopedBatch
    {
    public:
        explicit ScopedBatch(AbstractRenderer* renderer)
            : _renderer(renderer)
        {
            if (_renderer)
                _renderer->begin();
        }

        ScopedBatch(const ScopedBatch&) = delete;
        ScopedBatch& operator=(const ScopedBatch&) = delete;
        ScopedBatch(ScopedBatch&&) = delete;
        ScopedBatch& operator=(ScopedBatch&&) = delete;

        ~ScopedBatch()
        {
            if (_renderer)
                _renderer->end();
        }

        /**
         * Draws text at the specified position.
         */
        template <cl7::any_string_view_like Tstring_view_like>
        void draw_text(Tstring_view_like&& text, Font* font, const TextStyle* text_style = nullptr)
        {
            if (_renderer)
                _renderer->draw_text(std::forward<Tstring_view_like>(text), font, text_style);
        }

        /**
         * Forces the submission of all currently batched geometry. This can be called
         * manually if external render state changes, or when mixing with other
         * rendering systems that may modify render state.
         */
        void flush()
        {
            if (_renderer)
                _renderer->flush();
        }

        /**
         * Submits any queued geometry and finalizes the current text rendering batch.
         * This implicitly calls `flush`.
         */
        void end()
        {
            if (_renderer)
                _renderer->end();
            _renderer = nullptr;
        }

    private:
        AbstractRenderer* _renderer;
    };



    virtual ~AbstractRenderer() noexcept = default;



    /**
     * Creates a new `ScopedBatch` object to manage a text rendering batch.
     *
     * Use this function to start a new batch. The returned `ScopedBatch` object
     * will automatically call `end` when it goes out of scope, unless `end` is
     * called explicitly beforehand.
     */
    ScopedBatch begin_batch();



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
    void begin();

    /**
     * Submits any queued geometry and finalizes the current text rendering batch.
     * This implicitly calls `flush`.
     *
     * For greater convenience, the RAII helper `ScopedBatch` should be used via
     * `begin_batch`, which ultimately calls `end` automatically.
     */
    void end();

    /**
     * Forces the submission of all currently batched geometry. This can be called
     * manually if external render state changes, or when mixing with other
     * rendering systems that may modify render state.
     */
    void flush();



    /**
     * Draws text at the specified position.
     *
     * If no active text rendering batch is open, this function automatically calls
     * `begin` and `end` around the draw operation.
     */
    template <cl7::any_string_view_like Tstring_view_like>
    void draw_text(Tstring_view_like&& text, Font* font, const TextStyle* text_style = nullptr)
    {
        auto sv = cl7::make_string_view(std::forward<Tstring_view_like>(text));
        using codepoint_iterator = cl7::text::codec::codepoint_iterator<cl7::char_type_of_t<Tstring_view_like>>;

        _codepoints.clear();
        for (codepoint_iterator it(sv); it != codepoint_iterator(); ++it)
            _codepoints.push_back(*it);

        // Tha actual batching is managed in `_draw_codepoints`.

        _draw_codepoints(_codepoints, font, text_style);
    }



protected:
    /** The state to use for a single drawing invocation. */
    struct State
    {
        Font::Access font_access;
        FontMetrics font_metrics;

        TextStyle text_style;
        TextMetrics text_metrics;

        ml7::Vector2f cursor;
    };



private:
    void _draw_codepoints(const std::vector<cl7::text::codec::codepoint>& codepoints, Font* font, const TextStyle* text_style);

    void _emit_codepoint(cl7::text::codec::codepoint codepoint, State& state);

    virtual void _before_begin() {}
    virtual void _after_end() {}
    virtual void _do_flush() {}

    virtual void _emit_glyph(const Glyph& glyph, const State& state) = 0;



    /** The default text style to use when no other text style is specified. */
    TextStyle _default_text_style = {};

    /** The "flag"/counter specifying whether an active text rendering batch is open. */
    unsigned _batch_depth = 0;

    /**
     * Serves as a "temporary" buffer for the text represented as (Unicode) code
     * points of the current drawing invocation to avoid having to allocate new
     * memory each time.
     */
    std::vector<cl7::text::codec::codepoint> _codepoints;

}; // class AbstractRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_ABSTRACTRENDERER_H
