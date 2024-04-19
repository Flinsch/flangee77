#pragma once
#ifndef XL7_GRAPHICS_SHADERS_TEXTURESAMPLERTABLE_H
#define XL7_GRAPHICS_SHADERS_TEXTURESAMPLERTABLE_H

#include "./TextureSamplerDeclaration.h"

#include <CoreLabs/string.h>

#include <map>



namespace xl7 {
namespace graphics {
namespace shaders {



typedef std::map<cl7::astring, TextureSamplerDeclaration, std::less<>> TextureSamplerTable;



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_TEXTURESAMPLERTABLE_H
