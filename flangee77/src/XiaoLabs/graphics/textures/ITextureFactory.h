#ifndef XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H
#define XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H

#include "./Texture2D.h"
#include "./Texture3D.h"
#include "./Texture2DArray.h"
#include "./Cubemap.h"



namespace xl7::graphics::textures {



class TextureManager;



class ITextureFactory
{
public:
    virtual ~ITextureFactory() = default;

    virtual Texture2D* create_texture_2d(const resources::Resource::CreateContext& ctx, const Texture2DDesc& desc) = 0;
    virtual Texture3D* create_texture_3d(const resources::Resource::CreateContext& ctx, const Texture3DDesc& desc) = 0;
    virtual Texture2DArray* create_texture_2d_array(const resources::Resource::CreateContext& ctx, const Texture2DArrayDesc& desc) = 0;
    virtual Cubemap* create_cubemap(const resources::Resource::CreateContext& ctx, const CubemapDesc& desc) = 0;

}; // class ITextureFactory



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_ITEXTUREFACTORY_H
