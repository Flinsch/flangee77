#ifndef XL7_GRAPHICS_STATES_WINDINGORDER_H
#define XL7_GRAPHICS_STATES_WINDINGORDER_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Determines the orientation of the triangles. */
enum struct WindingOrder
{
    /** Consider a triangle front-facing if its vertices are clockwise on the render target (consider back-facing if they are counter-clockwise). */
    Clockwise,
    /** Consider a triangle front-facing if its vertices are counter-clockwise on the render target (consider back-facing if they are clockwise). */
    CounterClockwise,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_WINDINGORDER_H
