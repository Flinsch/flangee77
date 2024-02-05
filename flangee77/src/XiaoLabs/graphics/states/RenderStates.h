#pragma once
#ifndef XL7_GRAPHICS_STATES_RENDERSTATES_H
#define XL7_GRAPHICS_STATES_RENDERSTATES_H
#include "./AbstractStates.h"



namespace xl7 {
namespace graphics {
namespace states {



class RenderStates
    : public AbstractStates
{

private:
    enum Type : size_t
    {
        FILL_MODE,
        CULL_MODE,
        WINDING_ORDER,

        _TYPE_COUNT,
    };

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



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    StateArray<unsigned long, _TYPE_COUNT> _states;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    FillMode get_fill_mode() const { return _states.get_value( FILL_MODE, FillMode::Solid ); }
    CullMode get_cull_mode() const { return _states.get_value( CULL_MODE, CullMode::Back ); }
    WindingOrder get_winding_order() const { return _states.get_value( WINDING_ORDER, WindingOrder::Clockwise ); }

public:
    void set_fill_mode(FillMode fill_mode) { _states.set_value( FILL_MODE, fill_mode ); }
    void set_cull_mode(CullMode cull_mode) { _states.set_value( CULL_MODE, cull_mode ); }
    void set_winding_order(WindingOrder winding_order) { _states.set_value( WINDING_ORDER, winding_order ); }

}; // class RenderStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_RENDERSTATES_H
