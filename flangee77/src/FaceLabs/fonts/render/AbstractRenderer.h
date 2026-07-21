#ifndef FL7_FONTS_RENDER_ABSTRACTRENDERER_H
#define FL7_FONTS_RENDER_ABSTRACTRENDERER_H

#include "../Font.h"
#include "../FontMetrics.h"
#include "../IconRun.h"
#include "../TextMetrics.h"
#include "../TextStyle.h"
#include "./StyleRun.h"

#include <CoreLabs/string.h>

#include "CoreLabs/text/codec/codepoint_iterator.h"

#include <span>



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
        void draw_text(Tstring_view_like&& text, Font* font, const TextStyle* text_style = nullptr, ml7::Vector2f position = {}, std::span<const StyleRun> style_runs = {}, std::span<const IconRun> icon_runs = {})
        {
            if (_renderer)
                _renderer->draw_text(std::forward<Tstring_view_like>(text), font, text_style, position, style_runs, icon_runs);
        }

        /**
         * Draws text laid out within the specified box.
         */
        template <cl7::any_string_view_like Tstring_view_like>
        void draw_text_in_box(Tstring_view_like&& text, Font* font, const TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size, std::span<const StyleRun> style_runs = {}, std::span<const IconRun> icon_runs = {})
        {
            if (_renderer)
                _renderer->draw_text_in_box(std::forward<Tstring_view_like>(text), font, text_style, box_position, box_size, style_runs, icon_runs);
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
     * Draws text at the specified position (screen pixel space, y-down), honoring
     * `text_style`'s horizontal/vertical alignment by treating `position` as the
     * corresponding anchor point (e.g., with `Center` alignment, `position` is the
     * horizontal center of the text; with `Baseline` alignment, `position` is on
     * the first line's baseline, etc.). Multi-line text only results from explicit
     * line breaks in `text`, not from word-wrapping (there is no box width to wrap
     * against). `Justify` has no effect and behaves like `Left`.
     *
     * If no active text rendering batch is open, begin/end are called automatically.
     *
     * `style_runs` (if any) override `text_style`'s (inherited) `GlyphStyle` for
     * the code point ranges they cover. See `StyleRun` for the indexing convention
     * and ordering requirements.
     *
     * `icon_runs` (if any) each replace a single code point position with an inline
     * icon, for both layout (advance width) and rendering. See `IconRun` for the
     * indexing convention and ordering requirements.
     */
    template <cl7::any_string_view_like Tstring_view_like>
    void draw_text(Tstring_view_like&& text, Font* font, const TextStyle* text_style = nullptr, ml7::Vector2f position = {}, std::span<const StyleRun> style_runs = {}, std::span<const IconRun> icon_runs = {})
    {
        _extract_codepoints(std::forward<Tstring_view_like>(text));
        _draw_codepoints_in_box(_codepoints, font, text_style, position, ml7::Vector2f{}, style_runs, icon_runs);
    }

    /**
     * Draws text laid out within the specified box (screen pixel space, y-down),
     * honoring `text_style`'s horizontal/vertical alignment and, for
     * `WrapMode::Word`, wrapping lines to fit `box_size.x`.
     *
     * A `box_size` component of 0 (or less) means "unconstrained" on that axis.
     *
     * If no active text rendering batch is open, begin/end are called automatically.
     *
     * `style_runs` (if any) override `text_style`'s (inherited) `GlyphStyle` for
     * the code point ranges they cover. See `StyleRun` for the indexing convention
     * and ordering requirements.
     *
     * `icon_runs` (if any) each replace a single code point position with an inline
     * icon, for both layout (advance width) and rendering. See `IconRun` for the
     * indexing convention and ordering requirements.
     */
    template <cl7::any_string_view_like Tstring_view_like>
    void draw_text_in_box(Tstring_view_like&& text, Font* font, const TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size, std::span<const StyleRun> style_runs = {}, std::span<const IconRun> icon_runs = {})
    {
        _extract_codepoints(std::forward<Tstring_view_like>(text));
        _draw_codepoints_in_box(_codepoints, font, text_style, box_position, box_size, style_runs, icon_runs);
    }



protected:
    /** The state to use for a single drawing invocation. */
    struct State
    {
        Font::Access font_access;
        FontMetrics font_metrics;

        TextStyle text_style;
        TextMetrics text_metrics;

        /** The per-glyph style for the code point currently being emitted (text_style's, unless overridden by a StyleRun). */
        GlyphStyle current_glyph_style;

        ml7::Vector2f cursor;

        ml7::Vector2f box_position;
        ml7::Vector2f box_size;
    };



private:
    template <cl7::any_string_view_like Tstring_view_like>
    void _extract_codepoints(Tstring_view_like&& text)
    {
        auto sv = cl7::make_string_view(std::forward<Tstring_view_like>(text));
        using codepoint_iterator = cl7::text::codec::codepoint_iterator<cl7::char_type_of_t<Tstring_view_like>>;

        _codepoints.clear();
        for (codepoint_iterator it(sv); it != codepoint_iterator(); ++it)
            _codepoints.push_back(*it);
    }

    void _draw_codepoints_in_box(const std::vector<cl7::text::codec::codepoint>& codepoints, Font* font, const TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size, std::span<const StyleRun> style_runs, std::span<const IconRun> icon_runs);

    void _emit_codepoint(cl7::text::codec::codepoint codepoint, State& state);

    virtual void _before_begin() {}
    virtual void _after_end() {}
    virtual void _do_flush() {}

    virtual void _emit_glyph(const Glyph& glyph, const State& state) = 0;

    virtual void _emit_background(ml7::Vector2f position, ml7::Vector2f size, const State& state) {}

    virtual void _emit_icon(const Icon& icon, const State& state) {}



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
