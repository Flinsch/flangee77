#ifndef XL7_GRAPHICS_STATES_RASTERIZERSTATEDESC_H
#define XL7_GRAPHICS_STATES_RASTERIZERSTATEDESC_H

#include "./FillMode.h"
#include "./CullMode.h"
#include "./WindingOrder.h"



namespace xl7::graphics::states {



struct RasterizerStateDesc
{
    /** Determines the fill mode to use when rendering. */
    FillMode fill_mode  = FillMode::Solid;
    /** Indicates which triangles should be discarded (culled). */
    CullMode cull_mode  = CullMode::Back;
    /** Determines the orientation of the triangles. */
    WindingOrder winding_order  = WindingOrder::Clockwise;
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_RASTERIZERSTATEDESC_H
