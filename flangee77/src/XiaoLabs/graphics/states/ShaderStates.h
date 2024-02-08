#pragma once
#ifndef XL7_GRAPHICS_STATES_SHADERSTATES_H
#define XL7_GRAPHICS_STATES_SHADERSTATES_H
#include "./AbstractStates.h"

#include "../shaders/VertexShader.h"
#include "../shaders/PixelShader.h"



namespace xl7 {
namespace graphics {
namespace states {



class ShaderStates
    : public AbstractStates
{

    // #############################################################################
    // Attributes
    // #############################################################################
private:
    SingleState<resources::ResourceID> _shader_id;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    resources::ResourceID get_shader_id() const { return _shader_id.get_value(); }

public:
    void set_shader(resources::ResourceID shader_id) { _shader_id.set_value( shader_id ); }

}; // class ShaderStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_SHADERSTATES_H
