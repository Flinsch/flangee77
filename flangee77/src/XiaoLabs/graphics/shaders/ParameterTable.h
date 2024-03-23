#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H
#define XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H

#include "./ParameterDesc.h"

#include <CoreLabs/string.h>

#include <map>



namespace xl7 {
namespace graphics {
namespace shaders {



typedef std::map<cl7::astring, ParameterDesc, std::less<>> ParameterTable;



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H
