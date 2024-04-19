#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTTABLE_H
#define XL7_GRAPHICS_SHADERS_CONSTANTTABLE_H

#include "./ConstantDeclaration.h"

#include <CoreLabs/string.h>

#include <map>



namespace xl7 {
namespace graphics {
namespace shaders {



typedef std::map<cl7::astring, ConstantDeclaration, std::less<>> ConstantTable;



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTTABLE_H
