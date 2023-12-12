#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREMANAGER_H
#include "../../ResourceManager.h"



namespace xl7 {
namespace graphics {
namespace textures {



class TextureManager
    : public ResourceManager
{

protected:
    static void _destroy(TextureManager* texture_manager) { delete texture_manager; }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    TextureManager();

    /**
     * Destructor.
     */
    virtual ~TextureManager();

private:
    /** Copy constructor. */
    TextureManager(const TextureManager&) = delete;
    /** Copy assignment operator. */
    TextureManager& operator = (const TextureManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:



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
