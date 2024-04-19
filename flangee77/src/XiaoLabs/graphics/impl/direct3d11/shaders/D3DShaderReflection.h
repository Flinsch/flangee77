#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DSHADERREFLECTION_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DSHADERREFLECTION_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/ConstantBufferTable.h"
#include "../../../shaders/TextureSamplerTable.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class D3DShaderReflection
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Builds a constant buffer table based on the specified bytecode.
     */
    xl7::graphics::shaders::ConstantBufferTable build_constant_buffer_table(const xl7::graphics::shaders::ShaderCode& bytecode);

    /**
     * Builds a texture/sampler table based on the specified bytecode.
     */
    xl7::graphics::shaders::TextureSamplerTable build_texture_sampler_table(const xl7::graphics::shaders::ShaderCode& bytecode);

}; // class D3DShaderReflection



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DSHADERREFLECTION_H
