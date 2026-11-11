#include "AbstractRenderer.h"

#include "../Face.h"
#include "../HasBackground.h"
#include "../HasImage.h"
#include "../HasText.h"

#include <algorithm>



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
     * its faces drawn. Traversal order/strategy is entirely up to the concrete
     * renderer.
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
    void AbstractRenderer::draw_text(cl7::u32string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position)
    {
        ScopedFrame implicit_frame(_frame_depth == 0 ? this : nullptr);
        _draw_text_impl(text, font, text_style, position);
    }

    /**
     * Draws text laid out within the specified box (screen pixel space, y-down).
     * See `fl7::fonts::render::AbstractRenderer::draw_text_in_box` for the exact
     * positioning/alignment/wrapping contract.
     */
    void AbstractRenderer::draw_text_in_box(cl7::u32string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
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



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Draws the given face itself (not its children, if any, that's up to each
     * backend's own traversal in `_render_faces_impl`), clipped to its own declared
     * bounds (`face.get_size()`) regardless of what it actually draws: its
     * background (see `BackgroundHelper`) if it's a `HasBackground` (always over its
     * own full bounds, NOT inset by padding below), then, if it's a `HasImage`, its
     * image (see `ImageHelper`), then, if it's a `HasText`, its selection highlight
     * (see `SelectionHelper`), its text (see `TextHelper`), and its caret (see
     * `CaretHelper`), always in that (back-to-front) order, the latter three shifted
     * by `get_effective_padding()`'s own left/top on top of `get_text_scroll_offset()`,
     * and laid out against `get_text_box_size()` (narrower than `get_size()` while a
     * scroll bar is shown; see `faces::TextField`'s multiline mode), itself narrowed
     * further by that same padding when `get_text_box_size()` returns nothing (a face
     * overriding it, like `faces::TextField`, is assumed to already have accounted
     * for its own padding there). Backend-agnostic (talks only to this class's own
     * public drawing methods above), so shared by every `AbstractRenderer`
     * implementation. Call this from within your own traversal wherever you decide
     * to actually draw a given face.
     */
    void AbstractRenderer::_draw_face(const Face& face, ml7::Vector2f absolute_position)
    {
        push_clip_rect(absolute_position, absolute_position + face.get_size());

        if (dynamic_cast<const HasBackground*>(&face))
            _background_helper.draw(this, face, absolute_position);

        if (const auto* has_image = dynamic_cast<const HasImage*>(&face))
        {
            if (const Image* image = has_image->get_image())
                _image_helper.draw(this, *image, face, absolute_position, has_image->get_image_scale_mode());
        }

        if (const auto* has_text = dynamic_cast<const HasText*>(&face))
        {
            const Padding padding = face.get_effective_padding();
            const ml7::Vector2f content_position = absolute_position + ml7::Vector2f{padding.left, padding.top};
            const ml7::Vector2f default_text_box_size = {
                std::max(0.0f, face.get_size().x - padding.left - padding.right),
                std::max(0.0f, face.get_size().y - padding.top - padding.bottom),
            };

            const ml7::Vector2f text_scroll_offset = has_text->get_text_scroll_offset();
            const ml7::Vector2f text_box_size = has_text->get_text_box_size().value_or(default_text_box_size);
            _selection_helper.draw(this, has_text->get_display_text(), face, content_position, text_scroll_offset, text_box_size);
            _text_helper.draw(this, has_text->get_display_text(), face, content_position, text_scroll_offset, text_box_size);
            _caret_helper.draw(this, has_text->get_display_text(), face, content_position, text_scroll_offset, text_box_size);
        }

        pop_clip_rect();
    }



} // namespace fl7::gui::render
