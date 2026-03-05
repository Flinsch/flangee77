#ifndef XL7_GRAPHICS_SURFACES_SURFACEDESC_H
#define XL7_GRAPHICS_SURFACES_SURFACEDESC_H

#include <CoreLabs/root.h>



namespace xl7::graphics::surfaces {



struct SurfaceDesc
{
    /** The width of the surface, in pixels. */
    unsigned width;
    /** The height of the surface, in pixels. */
    unsigned height;
};



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_SURFACEDESC_H
