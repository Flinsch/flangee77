#ifndef XL7_GRAPHICS_MESHES_CLIPPEDQUAD_H
#define XL7_GRAPHICS_MESHES_CLIPPEDQUAD_H

#include <MathLabs/Vector2.h>

#include <optional>



namespace xl7::graphics::meshes {



/**
 * An axis-aligned quad, given by its position rect and corresponding UV rect,
 * as produced by clipping another quad against a clip rect.
 */
struct ClippedQuad
{
    ml7::Vector2f position_min, position_max;
    ml7::Vector2f uv_min, uv_max;

    /**
     * Clips an axis-aligned quad (given by its position rect and corresponding UV
     * rect) against a clip rect, returning the clipped quad with proportionally
     * remapped UVs, or `std::nullopt` if nothing remains visible (fully outside the
     * clip rect, or clipped down to zero width/height).
     *
     * Pass `-infinity`/`+infinity` for `clip_min`/`clip_max` on an axis to leave
     * that axis unconstrained.
     */
    static std::optional<ClippedQuad> clip(ml7::Vector2f position_min, ml7::Vector2f position_max, ml7::Vector2f uv_min, ml7::Vector2f uv_max, ml7::Vector2f clip_min, ml7::Vector2f clip_max);

}; // struct ClippedQuad



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_CLIPPEDQUAD_H
