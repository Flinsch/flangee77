#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDESC_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDESC_H

#include "./ConstantBufferLayout.h"



namespace xl7::graphics::shaders {



struct ConstantBufferDesc
{
    /** The layout specification of the constant buffer. */
    ConstantBufferLayout layout;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDESC_H
