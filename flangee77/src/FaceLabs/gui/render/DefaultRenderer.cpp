#include "DefaultRenderer.h"

#include "../Collection.h"
#include "../Window.h"



namespace fl7::gui::render {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    DefaultRenderer::DefaultRenderer(fonts::render::AbstractRenderer* text_renderer)
        : _text_renderer(text_renderer)
    {
        assert(_text_renderer);
    }



    // #############################################################################
    // AbstractRenderer Implementations
    // #############################################################################

    void DefaultRenderer::_begin_frame_impl()
    {
        _quad_renderer.begin();
        _text_renderer->begin();
    }

    void DefaultRenderer::_end_frame_impl()
    {
        // End in reverse order, symmetry with begin.
        _text_renderer->end();
        _quad_renderer.end();
    }

    void DefaultRenderer::_render_faces_impl(const std::vector<std::unique_ptr<Face>>& top_level_faces)
    {
        for (const auto& face : top_level_faces)
            _render_face_recursive(*face, {});
    }

    void DefaultRenderer::_draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color)
    {
        _quad_renderer.draw_rect(position_min, position_max, color);
    }

    void DefaultRenderer::_draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint)
    {
        _quad_renderer.draw_textured_rect(position_min, position_max, texture_id, uv_min, uv_max, tint);
    }

    void DefaultRenderer::_draw_text_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position)
    {
        _text_renderer->draw_text(text, font, text_style, position);
    }

    void DefaultRenderer::_draw_text_in_box_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size)
    {
        _text_renderer->draw_text_in_box(text, font, text_style, box_position, box_size);
    }

    void DefaultRenderer::_push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max)
    {
        _quad_renderer.push_clip_rect(clip_min, clip_max);
    }

    void DefaultRenderer::_pop_clip_rect_impl()
    {
        _quad_renderer.pop_clip_rect();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Recursively walks the given face (and, if it's a collection, its children) at
     * the given absolute (screen) position.
     */
    void DefaultRenderer::_render_face_recursive(const Face& face, ml7::Vector2f parent_absolute_position)
    {
        if (!face.is_visible())
            return;

        const ml7::Vector2f absolute_position = parent_absolute_position + face.get_position();

        if (dynamic_cast<const Window*>(&face))
            _draw_background(face, absolute_position);

        // TODO: dispatch to further per-face-type drawing logic here, once more
        // concrete widget types exist (Button, Label, Panel, ...).

        if (const auto* collection = dynamic_cast<const Collection*>(&face))
        {
            for (const auto& child : collection->get_children())
                _render_face_recursive(*child, absolute_position);
        }
    }

    /**
     * Draws a face's flat background (Style::background_color) over its full
     * bounds. Shared by any face type whose chrome is "just a colored rect"
     * (Window now, Panel likely later).
     */
    void DefaultRenderer::_draw_background(const Face& face, ml7::Vector2f absolute_position)
    {
        draw_rect(absolute_position, absolute_position + face.get_size(), face.get_effective_style().background_color);
    }



} // namespace fl7::gui::render
