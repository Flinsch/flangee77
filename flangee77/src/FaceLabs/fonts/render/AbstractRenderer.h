#ifndef FL7_FONTS_RENDER_ABSTRACTRENDERER_H
#define FL7_FONTS_RENDER_ABSTRACTRENDERER_H

#include "../Font.h"
#include "../FontMetrics.h"
#include "../TextMetrics.h"
#include "../TextStyle.h"

#include <CoreLabs/string.h>

#include "CoreLabs/text/codec/codepoint_iterator.h"



namespace fl7::fonts::render {



class AbstractRenderer
{

public:
    struct AutoBeginEnd
    {
        AbstractRenderer* renderer;

        AutoBeginEnd(AbstractRenderer* renderer) noexcept : renderer(renderer) { if (renderer) renderer->begin(); }

        AutoBeginEnd(const AutoBeginEnd&) = delete;
        AutoBeginEnd& operator=(const AutoBeginEnd&) = delete;
        AutoBeginEnd(AutoBeginEnd&&) = delete;
        AutoBeginEnd& operator=(AutoBeginEnd&&) = delete;

        ~AutoBeginEnd() { if (renderer) renderer->end(); }
    };



    virtual ~AbstractRenderer() noexcept = default;



    /**
     * Begins a new text rendering batch. Must be balled before issuing any draw
     * commands if batching manually. Typically followed by multiple calls to
     * `draw_text`, and then a final call to `end`. If you only call `draw_text`
     * occasionally, it is not necessary to call `begin` or `end` manually; the
     * renderer will manage batching internally.
     */
    void begin();

    /**
     * Submits any queued geometry and finalizes the current text rendering batch.
     * This implicitly calls `flush`.
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

        AutoBeginEnd auto_begin_end(_is_batching ? nullptr : this);

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

    /** The flag specifying whether an active text rendering batch is open. */
    bool _is_batching = false;

    /**
     * Serves as a "temporary" buffer for the text represented as (Unicode) code
     * points of the current drawing invocation to avoid having to allocate new
     * memory each time.
     */
    std::vector<cl7::text::codec::codepoint> _codepoints;

}; // class AbstractRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_ABSTRACTRENDERER_H
