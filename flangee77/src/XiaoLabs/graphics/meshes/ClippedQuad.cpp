#include "ClippedQuad.h"

#include <algorithm>



namespace xl7::graphics::meshes {



    /**
     * Clips an axis-aligned quad (given by its position rect and corresponding UV
     * rect) against a clip rect, returning the clipped quad with proportionally
     * remapped UVs, or `std::nullopt` if nothing remains visible (fully outside the
     * clip rect, or clipped down to zero width/height).
     *
     * Pass `-infinity`/`+infinity` for `clip_min`/`clip_max` on an axis to leave
     * that axis unconstrained.
     */
    std::optional<ClippedQuad> ClippedQuad::clip(ml7::Vector2f position_min, ml7::Vector2f position_max, ml7::Vector2f uv_min, ml7::Vector2f uv_max, ml7::Vector2f clip_min, ml7::Vector2f clip_max)
    {
        const float width = position_max.x - position_min.x;
        const float height = position_max.y - position_min.y;
        if (width <= 0.0f || height <= 0.0f)
            return std::nullopt;

        const float clamped_left = std::max(position_min.x, clip_min.x);
        const float clamped_right = std::min(position_max.x, clip_max.x);
        const float clamped_top = std::max(position_min.y, clip_min.y);
        const float clamped_bottom = std::min(position_max.y, clip_max.y);

        if (clamped_right <= clamped_left || clamped_bottom <= clamped_top)
            return std::nullopt;

        // Remap UVs by the same interpolation fraction the position clamp
        // applied on each axis (the position-to-UV mapping is affine).
        const float u0 = uv_min.x + (uv_max.x - uv_min.x) * (clamped_left - position_min.x) / width;
        const float u1 = uv_min.x + (uv_max.x - uv_min.x) * (clamped_right - position_min.x) / width;
        const float v0 = uv_min.y + (uv_max.y - uv_min.y) * (clamped_top - position_min.y) / height;
        const float v1 = uv_min.y + (uv_max.y - uv_min.y) * (clamped_bottom - position_min.y) / height;

        return ClippedQuad{
            .position_min = {clamped_left, clamped_top},
            .position_max = {clamped_right, clamped_bottom},
            .uv_min = {u0, v0},
            .uv_max = {u1, v1},
        };
    }



} // namespace xl7::graphics::meshes
