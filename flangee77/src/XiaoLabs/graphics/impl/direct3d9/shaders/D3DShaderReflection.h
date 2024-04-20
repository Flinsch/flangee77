#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/ReflectionResult.h"

#include <vector>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace shaders {



class D3DShaderReflection
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    bool reflect(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out);

}; // class D3DShaderReflection



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
