#pragma once
#ifndef XL7_GRAPHICS_SHADERS_REFLECTIONRESULT_H
#define XL7_GRAPHICS_SHADERS_REFLECTIONRESULT_H

#include "./ConstantBufferDeclaration.h"
#include "./TextureSamplerDeclaration.h"

#include <vector>



namespace xl7 {
namespace graphics {
namespace shaders {



struct ReflectionResult
{

    std::vector<ConstantBufferDeclaration> constant_buffer_declarations;
    std::vector<TextureSamplerDeclaration> texture_sampler_declarations;

}; // struct ReflectionResult



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_REFLECTIONRESULT_H
