#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/ReflectionResult.h"



namespace xl7::graphics::impl::direct3d9::shaders {



class D3DShaderReflection
{

public:

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    static bool reflect(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out);

}; // class D3DShaderReflection



} // namespace xl7::graphics::impl::direct3d9::shaders

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
