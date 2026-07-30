#include "AbstractRenderer.h"

#include "../Face.h"



namespace fl7::gui::render {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Begins a new frame. If you only call `draw_rect`, `draw_text`, etc.
     * occasionally, it is not necessary to call `begin_frame`/`end_frame` manually.
     * The renderer will manage this internally.
     *
     * For greater convenience, the RAII helper `ScopedFrame` should be used via
     * `begin_scoped_frame`.
     */
    void AbstractRenderer::begin_frame()
    {
        if (_frame_depth == 0)
            _begin_frame_impl();

        ++_frame_depth;
    }

    /**
     * Ends the current frame, submitting any queued/batched geometry.
     *
     * For greater convenience, the RAII helper `ScopedFrame` should be used`via
     * `begin_scoped_frame`, which ultimately calls `end_frame` automatically.
     */
    void AbstractRenderer::end_frame()
    {
        assert(_frame_depth > 0);
        if (_frame_depth == 0)
            return;

        if (--_frame_depth == 0)
            _end_frame_impl();
    }

    /**
     * Renders the given top-level faces (and, transitively, their descendants) as a
     * complete frame. This is the single entry point through which a shell gets
     * its faces drawn. Traversal order/strategy and per-face-type drawing logic are
     * entirely up to the concrete renderer.
     */
    void AbstractRenderer::render_faces(const std::vector<std::unique_ptr<Face>>& top_level_faces)
    {
        auto frame = begin_scoped_frame();
        _render_faces_impl(top_level_faces);
    }

    /**
     * Draws a flat-colored, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see `push_clip_rect`).
     */
    void AbstractRenderer::draw_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color)
    {
        ScopedFrame implicit_frame(_frame_depth == 0 ? this : nullptr);
        _draw_rect_impl(position_min, position_max, color);
    }

    /**
     * Draws a simple-textured, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see `push_clip_rect`).
     * The sampled texture color is modulated by `tint`.
     */
    void AbstractRenderer::draw_textured_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint)
    {
        ScopedFrame implicit_frame(_frame_depth == 0 ? this : nullptr);
        _draw_textured_rect_impl(position_min, position_max, texture_id, uv_min, uv_max, tint);
    }

    /**
     * Draws text at the specified position (screen pixel space, y-down). See
     * `fl7::fonts::render::AbstractRenderer::draw_text` for the exact
     * positioning/alignment contract.
     */
    void AbstractRenderer::draw_text(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position)
    {
        ScopedFrame implicit_frame(_frame_depth == 0 ? this : nullptr);
        _draw_text_impl(text, font, text_style, position);
    }

    /**
     * Draws text laid out within the specified box (screen pixel space, y-down).
     * See `fl7::fonts::render::AbstractRenderer::draw_text_in_box` for the exact
     * positioning/alignment/wrapping contract.
     */
    void AbstractRenderer::draw_text_in_box(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
    {
        ScopedFrame implicit_frame(_frame_depth == 0 ? this : nullptr);
        _draw_text_in_box_impl(text, font, text_style, box_position, box_size);
    }

    /**
     * Pushes a new clip rect, intersected with the current one (there is no clip
     * rect initially, i.e., unbounded). Drawing while this clip rect is active is
     * clipped against it.
     */
    void AbstractRenderer::push_clip_rect(ml7::Vector2f clip_min, ml7::Vector2f clip_max)
    {
        _push_clip_rect_impl(clip_min, clip_max);
    }

    /**
     * Pops the most recently pushed clip rect, restoring the previous one.
     */
    void AbstractRenderer::pop_clip_rect()
    {
        _pop_clip_rect_impl();
    }



} // namespace fl7::gui::render
