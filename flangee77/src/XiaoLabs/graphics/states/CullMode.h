#ifndef XL7_GRAPHICS_STATES_CULLMODE_H
#define XL7_GRAPHICS_STATES_CULLMODE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Indicates which triangles should be discarded (culled). */
enum struct CullMode
{
    /** Always draw all triangles. */
    None,
    /** Do not draw triangles that are front-facing. */
    Front,
    /** Do not draw triangles that are back-facing. */
    Back,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_CULLMODE_H
