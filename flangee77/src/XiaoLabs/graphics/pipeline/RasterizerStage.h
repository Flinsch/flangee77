#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
#include "./AbstractStage.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class RasterizerStage
    : public AbstractStage
{

public:
    /** Determines the fill mode to use when rendering. */
    enum class FillMode
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

    /** Indicates which triangles should be discarded (culled). */
    enum class CullMode
    {
        /** Always draw all triangles. */
        None,
        /** Do not draw triangles that are front-facing. */
        Front,
        /** Do not draw triangles that are back-facing. */
        Back,
    };

    /** Determines the orientation of the triangles. */
    enum class WindingOrder
    {
        /** Consider a triangle front-facing if its vertices are clockwise on the render target (consider back-facing if they are counter-clockwise). */
        Clockwise,
        /** Consider a triangle front-facing if its vertices are counter-clockwise on the render target (consider back-facing if they are clockwise). */
        CounterClockwise,
    };

private:
    static constexpr unsigned DIRTY_FLAG_FILL_MODE      = 0x1;
    static constexpr unsigned DIRTY_FLAG_CULL_MODE      = 0x2;
    static constexpr unsigned DIRTY_FLAG_WINDING_ORDER  = 0x4;



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( fill_mode, FillMode, FillMode::Solid, DIRTY_FLAG_FILL_MODE );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( cull_mode, CullMode, CullMode::Back, DIRTY_FLAG_CULL_MODE );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( winding_order, WindingOrder, WindingOrder::Clockwise, DIRTY_FLAG_WINDING_ORDER );

}; // class RasterizerStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
