#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./ITextureFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace textures {



class TextureManager final
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static TextureManager* create(ITextureFactory* factory) { return new TextureManager( factory ); }
        static void destroy(TextureManager* manager) { delete manager; }
        friend class xl7::graphics::RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    TextureManager(ITextureFactory* factory);

    /**
     * Destructor.
     */
    virtual ~TextureManager();

private:
    /** Default constructor. */
    TextureManager() = delete;
    /** Copy constructor. */
    TextureManager(const TextureManager&) = delete;
    /** Copy assignment operator. */
    TextureManager& operator = (const TextureManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The texture factory.
     */
    ITextureFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Creates and acquires the specified 2D texture.
     */
    resources::ResourceID create_texture_2d(cl7::astring_view identifier, const Texture2D::Desc& desc, const ImageDataProvider& data_provider);

}; // class TextureManager



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
