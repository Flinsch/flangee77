#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H
#define XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H

#include "./Texture2D.h"
#include "./Texture3D.h"
#include "./Texture2DArray.h"
#include "./Cubemap.h"



namespace xl7 {
namespace graphics {
namespace textures {



class TextureManager;



class ITextureFactory
{
public:
    virtual ~ITextureFactory() = default;

    virtual xl7::graphics::textures::Texture2D* create_texture_2d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2D::Desc>& params) = 0;
    virtual xl7::graphics::textures::Texture3D* create_texture_3d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture3D::Desc>& params) = 0;
    virtual xl7::graphics::textures::Texture2DArray* create_texture_2d_array(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DArray::Desc>& params) = 0;
    virtual xl7::graphics::textures::Cubemap* create_cubemap(const resources::Resource::CreateParams<xl7::graphics::textures::Cubemap::Desc>& params) = 0;

}; // class ITextureFactory



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H
