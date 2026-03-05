#ifndef XL7_GRAPHICS_STATES_FILLMODE_H
#define XL7_GRAPHICS_STATES_FILLMODE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Determines the fill mode to use when rendering. */
enum struct FillMode
{
    /** Do not draw anything at all. */
    None,
    /** Draw the vertices as points. */
    Point,
    /** Draw lines connecting the vertices. */
    Wireframe,
    /** Fill the triangles formed by the vertices. */
    Solid,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_FILLMODE_H
