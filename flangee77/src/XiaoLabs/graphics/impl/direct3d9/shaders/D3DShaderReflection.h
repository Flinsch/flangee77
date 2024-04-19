#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/ConstantBufferDeclaration.h"
#include "../../../shaders/TextureSamplerDeclaration.h"

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
     * Builds constant buffer declarations based on the specified bytecode.
     */
    std::vector<xl7::graphics::shaders::ConstantBufferDeclaration> build_constant_buffer_declarations(const xl7::graphics::shaders::ShaderCode& bytecode);

    /**
     * Builds texture/sampler declarations based on the specified bytecode.
     */
    std::vector<xl7::graphics::shaders::TextureSamplerDeclaration> build_texture_sampler_declarations(const xl7::graphics::shaders::ShaderCode& bytecode);

}; // class D3DShaderReflection



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_D3DSHADERREFLECTION_H
