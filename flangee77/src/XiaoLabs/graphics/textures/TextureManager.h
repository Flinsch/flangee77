#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#include "../../ResourceManager.h"

#include "./ITextureFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace textures {



class TextureManager final
    : public ResourceManager
{

public:
    class Attorney
    {
        static TextureManager* create(ITextureFactory* factory) { return new TextureManager( factory ); }
        static void destroy(TextureManager* manager) { delete manager; }
        friend class RenderingDevice;
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



    // #############################################################################
    // Prototypes
    // #############################################################################
private:

}; // class TextureManager



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
