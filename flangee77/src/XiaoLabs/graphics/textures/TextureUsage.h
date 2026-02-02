#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREUSAGE_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREUSAGE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::textures {



/**
 * Describes the expected lifetime and update frequency of a texture resource.
 *
 * The selected usage guides how the backend manages memory placement, update paths,
 * and synchronization. Choosing a usage that does not match the actual update
 * behavior may result in reduced performance.
 */
enum struct TextureUsage
{
    /**
     * The texture is initialized once and never modified afterward.
     *
     * Intended for fully static textures (e.g., material properties loaded from
     * disk, precomputed cubemaps, etc.). Provides the strongest guarantee and
     * allows maximum optimization.
     */
    Immutable,

    /**
     * The texture is long-lived and usually static, but its contents may be replaced
     * or refreshed occasionally.
     *
     * This is the default choice for most textures that are not strictly
     * immutable (e.g., texture atlases, UI sprites, etc.). Compared to immutable,
     * this usage sacrifices some optimization potential in exchange for (limited)
     * mutability.
     */
    Default,

    /**
     * The texture contents are updated frequently (e.g., once per frame) while the
     * texture object itself remains stable.
     *
     * Intended for per-frame or near-per-frame updates of texture data (e.g., render
     * targets, dynamic shadows, procedural textures, video playback, etc.). Dynamic
     * textures favor update efficiency over maximal GPU locality.
     */
    Dynamic,
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREUSAGE_H
