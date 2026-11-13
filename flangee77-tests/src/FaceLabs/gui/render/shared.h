#ifndef F77_TESTS_FL7_GUI_RENDER_SHARED_H
#define F77_TESTS_FL7_GUI_RENDER_SHARED_H

#include <FaceLabs/gui/render/AbstractRenderer.h>

#include <optional>



namespace f77_tests::gui::render {



    /** Records the last draw_rect()/draw_textured_rect()/draw_text_in_box() call's arguments; everything else is a no-op. */
    class RecordingRenderer
        : public fl7::gui::render::AbstractRenderer
    {
    public:
        struct Rect
        {
            ml7::Vector2f min;
            ml7::Vector2f max;
            xl7::graphics::Color color;
        };

        struct TexturedRect
        {
            ml7::Vector2f min;
            ml7::Vector2f max;
            xl7::graphics::textures::Texture2D::Id texture_id;
            ml7::Vector2f uv_min;
            ml7::Vector2f uv_max;
            xl7::graphics::Color tint;
        };

        struct TextInBox
        {
            cl7::u32string text;
            fl7::fonts::Font* font;
            fl7::fonts::TextStyle text_style;
            ml7::Vector2f box_position;
            ml7::Vector2f box_size;
        };

        std::optional<Rect> last_rect;
        std::optional<TexturedRect> last_textured_rect;
        std::optional<TextInBox> last_text_in_box;

    private:
        void _begin_frame_impl() override {}
        void _end_frame_impl() override {}

        void _render_faces_impl(const std::vector<std::unique_ptr<fl7::gui::Face>>& top_level_faces) override {}

        void _draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color) override
        {
            last_rect = Rect{position_min, position_max, color};
        }

        void _draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint) override
        {
            last_textured_rect = TexturedRect{position_min, position_max, texture_id, uv_min, uv_max, tint};
        }

        void _draw_text_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f position) override {}

        void _draw_text_in_box_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size) override
        {
            last_text_in_box = TextInBox{cl7::u32string(text), font, text_style ? *text_style : fl7::fonts::TextStyle{}, box_position, box_size};
        }

        void _push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max) override {}
        void _pop_clip_rect_impl() override {}

    }; // class RecordingRenderer



} // namespace f77_tests::gui::render

#endif // F77_TESTS_FL7_GUI_RENDER_SHARED_H
