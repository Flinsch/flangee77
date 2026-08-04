#include "DefaultRenderer.h"

#include "../Button.h"
#include "../Collection.h"
#include "../Label.h"
#include "../NineSliceChrome.h"
#include "../Panel.h"
#include "../Window.h"

#include <algorithm>



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
        // Invoking `end` is what actually submits/flushes a renderer's batched draw calls,
        // so this order is what determines paint order (regardless of the order of `begin`):
        // quads first (rects, backgrounds, ...), text on top of them, not the other way around.
        _quad_renderer.end();
        _text_renderer->end();
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

        if (dynamic_cast<const Window*>(&face) || dynamic_cast<const Panel*>(&face) || dynamic_cast<const Button*>(&face))
            _draw_background(face, absolute_position);

        if (const auto* label = dynamic_cast<const Label*>(&face))
            _draw_text(label->get_text(), face, absolute_position);
        else if (const auto* button = dynamic_cast<const Button*>(&face))
            _draw_text(button->get_text(), face, absolute_position);

        // TODO: dispatch to further per-face-type drawing logic here, once more
        // concrete widget types exist (CheckBox, TextField, ...).

        if (const auto* collection = dynamic_cast<const Collection*>(&face))
        {
            for (const auto& child : collection->get_children())
                _render_face_recursive(*child, absolute_position);
        }
    }

    /**
     * Draws a face's background over its full bounds: a 9-sliced NineSliceChrome
     * sprite (see _draw_chrome) if Style::chrome is set, else a flat
     * Style::background_color rect. Shared by any face type with such a background
     * (Window, Panel, ...).
     */
    void DefaultRenderer::_draw_background(const Face& face, ml7::Vector2f absolute_position)
    {
        const Style& style = face.get_effective_style();

        if (style.chrome)
            _draw_chrome(*style.chrome, absolute_position, face.get_size());
        else
            draw_rect(absolute_position, absolute_position + face.get_size(), style.background_color);
    }

    /**
     * Draws the given chrome sprite 9-sliced over a destination rect at
     * absolute_position/size: corners at native size, edges stretched along their
     * one free axis, the center stretched along both.
     */
    void DefaultRenderer::_draw_chrome(const NineSliceChrome& chrome, ml7::Vector2f absolute_position, ml7::Vector2f size)
    {
        // Clamp insets so they never overlap/invert on a face smaller than the chrome's own border.
        const float inset_left = std::min(chrome.inset_left, size.x * 0.5f);
        const float inset_right = std::min(chrome.inset_right, size.x * 0.5f);
        const float inset_top = std::min(chrome.inset_top, size.y * 0.5f);
        const float inset_bottom = std::min(chrome.inset_bottom, size.y * 0.5f);

        const float dest_x[4] = {absolute_position.x, absolute_position.x + inset_left, absolute_position.x + size.x - inset_right, absolute_position.x + size.x};
        const float dest_y[4] = {absolute_position.y, absolute_position.y + inset_top, absolute_position.y + size.y - inset_bottom, absolute_position.y + size.y};

        const ml7::Vector2f uv_size = chrome.uv_max - chrome.uv_min;
        const float uv_x[4] = {chrome.uv_min.x, chrome.uv_min.x + (chrome.inset_left / chrome.sprite_size.x) * uv_size.x, chrome.uv_max.x - (chrome.inset_right / chrome.sprite_size.x) * uv_size.x, chrome.uv_max.x};
        const float uv_y[4] = {chrome.uv_min.y, chrome.uv_min.y + (chrome.inset_top / chrome.sprite_size.y) * uv_size.y, chrome.uv_max.y - (chrome.inset_bottom / chrome.sprite_size.y) * uv_size.y, chrome.uv_max.y};

        for (int row = 0; row < 3; ++row)
        {
            if (dest_y[row] >= dest_y[row + 1])
                continue;

            for (int col = 0; col < 3; ++col)
            {
                if (dest_x[col] >= dest_x[col + 1])
                    continue;

                draw_textured_rect(
                    {dest_x[col], dest_y[row]}, {dest_x[col + 1], dest_y[row + 1]},
                    chrome.texture_id,
                    {uv_x[col], uv_y[row]}, {uv_x[col + 1], uv_y[row + 1]},
                    {1.0f, 1.0f, 1.0f, 1.0f});
            }
        }
    }

    /**
     * Draws the given text (Style::font/text_style, taken from face) within face's
     * bounds. Shared by any face type that just displays a string of text over its
     * own area (Label, Button, ...).
     */
    void DefaultRenderer::_draw_text(cl7::u8string_view text, const Face& face, ml7::Vector2f absolute_position)
    {
        const Style& style = face.get_effective_style();
        draw_text_in_box(text, style.font, &style.text_style, absolute_position, face.get_size());
    }



} // namespace fl7::gui::render
