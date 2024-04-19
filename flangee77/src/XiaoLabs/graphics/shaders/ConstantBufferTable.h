#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERTABLE_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERTABLE_H

#include "./ConstantBufferDeclaration.h"

#include <CoreLabs/string.h>

#include <map>



namespace xl7 {
namespace graphics {
namespace shaders {



typedef std::map<cl7::astring, ConstantBufferDeclaration, std::less<>> ConstantBufferTable;



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERTABLE_H
