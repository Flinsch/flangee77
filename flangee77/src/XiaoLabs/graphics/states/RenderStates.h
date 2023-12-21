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
        FRONT_FACE_WINDING_ORDER,

        _TYPE_COUNT,
    };

public:
    enum class FillMode
    {
        None,
        Point,
        Wireframe,
        Solid,
    };

    enum class CullMode
    {
        None,
        Front,
        Back,
    };

    enum class FrontFaceWindingOrder
    {
        Clockwise,
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
    FrontFaceWindingOrder get_front_face_winding_order() const { return _states.get_value( FRONT_FACE_WINDING_ORDER, FrontFaceWindingOrder::Clockwise ); }

public:
    void set_fill_mode(FillMode fill_mode) { _states.set_value( FILL_MODE, fill_mode ); }
    void set_cull_mode(CullMode cull_mode) { _states.set_value( CULL_MODE, cull_mode ); }
    void set_front_face_winding_order(FrontFaceWindingOrder front_face_winding_order) { _states.set_value( FRONT_FACE_WINDING_ORDER, front_face_winding_order ); }

}; // class RenderStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_RENDERSTATES_H
