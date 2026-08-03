#ifndef FL7_GUI_RENDER_DEFAULTRENDERER_H
#define FL7_GUI_RENDER_DEFAULTRENDERER_H
#include "./AbstractRenderer.h"

#include <FaceLabs/fonts/render/AbstractRenderer.h>

#include <XiaoLabs/graphics/QuadRenderer.h>

#include <MathLabs/Vector2.h>



namespace fl7::gui {
class Face;
class Label;
} // namespace fl7::gui

namespace fl7::gui::render {



/**
 * The batteries-included `AbstractRenderer` implementation: draws (textured) rects
 * via an owned `xl7::graphics::QuadRenderer`, and text via a caller-supplied
 * `fl7::fonts::render::AbstractRenderer` (still a free choice of Bitmap/SDF/MSDF
 * backend, not hardcoded here either).
 */
class DefaultRenderer
    : public AbstractRenderer
{

public:
    explicit DefaultRenderer(fonts::render::AbstractRenderer* text_renderer);

    DefaultRenderer(const DefaultRenderer&) = delete;
    DefaultRenderer& operator=(const DefaultRenderer&) = delete;
    DefaultRenderer(DefaultRenderer&&) = delete;
    DefaultRenderer& operator=(DefaultRenderer&&) = delete;

    ~DefaultRenderer() override = default;



private:

    // #############################################################################
    // AbstractRenderer Implementations
    // #############################################################################

    void _begin_frame_impl() override;
    void _end_frame_impl() override;

    void _render_faces_impl(const std::vector<std::unique_ptr<Face>>& top_level_faces) override;

    void _draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color) override;
    void _draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint) override;
    void _draw_text_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position) override;
    void _draw_text_in_box_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size) override;

    void _push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max) override;
    void _pop_clip_rect_impl() override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Recursively walks the given face (and, if it's a collection, its children) at
     * the given absolute (screen) position.
     */
    void _render_face_recursive(const Face& face, ml7::Vector2f parent_absolute_position);

    /**
     * Draws a face's flat background (Style::background_color) over its full
     * bounds. Shared by any face type whose chrome is "just a colored rect"
     * (Window now, Panel likely later).
     */
    void _draw_background(const Face& face, ml7::Vector2f absolute_position);

    /**
     * Draws a label's text (Style::font/text_style) within its bounds.
     */
    void _draw_label(const Label& label, ml7::Vector2f absolute_position);



    // #############################################################################
    // Attributes
    // #############################################################################

    xl7::graphics::QuadRenderer _quad_renderer;

    /** Non-owning: caller decides the concrete font-rendering backend. */
    fonts::render::AbstractRenderer* _text_renderer;

}; // class DefaultRenderer



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_DEFAULTRENDERER_H
