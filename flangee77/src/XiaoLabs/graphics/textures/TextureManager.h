#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./ITextureFactory.h"



namespace xl7::graphics {
    class RenderingDevice;
namespace textures {



class TextureManager final
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static TextureManager* create(ITextureFactory* factory) { return new TextureManager(factory); }
        static void destroy(TextureManager* manager) { delete manager; }
        friend class xl7::graphics::RenderingDevice;
    };



    TextureManager() = delete;

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;



    /**
     * Creates and acquires the specified 2D texture.
     */
    Texture2D::Id create_texture_2d(cl7::u8string_view identifier, const Texture2D::Desc& desc, const ImageDataProvider& data_provider);



protected:
    explicit TextureManager(ITextureFactory* factory) : _factory(factory) {}
    ~TextureManager() override = default;



private:
    /**
     * The texture factory.
     */
    ITextureFactory* const _factory;

}; // class TextureManager



} // namespace textures
} // namespace xl7::graphics

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
