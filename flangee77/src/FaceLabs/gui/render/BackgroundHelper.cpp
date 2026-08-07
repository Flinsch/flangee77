#include "BackgroundHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"
#include "../NineSliceChrome.h"
#include "../Style.h"

#include <algorithm>



namespace fl7::gui::render {



    /**
     * Draws face's background at absolute_position via renderer.
     */
    void BackgroundHelper::draw(AbstractRenderer* renderer, const Face& face, ml7::Vector2f absolute_position)
    {
        const Style& style = face.get_effective_style();

        if (style.chrome)
            _draw_chrome(renderer, *style.chrome, absolute_position, face.get_size());
        else
            renderer->draw_rect(absolute_position, absolute_position + face.get_size(), style.background_color);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Draws the given chrome sprite 9-sliced over a destination rect at
     * absolute_position/size: corners at native size, edges stretched along their
     * one free axis, the center stretched along both.
     */
    void BackgroundHelper::_draw_chrome(AbstractRenderer* renderer, const NineSliceChrome& chrome, ml7::Vector2f absolute_position, ml7::Vector2f size)
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

                renderer->draw_textured_rect(
                    {dest_x[col], dest_y[row]}, {dest_x[col + 1], dest_y[row + 1]},
                    chrome.texture_id,
                    {uv_x[col], uv_y[row]}, {uv_x[col + 1], uv_y[row + 1]},
                    {1.0f, 1.0f, 1.0f, 1.0f});
            }
        }
    }



} // namespace fl7::gui::render
