#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTUREMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTUREMANAGERIMPL_H
#include "../../../textures/TextureManager.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace textures {



class TextureManagerImpl final
    : public xl7::graphics::textures::TextureManager
{

public:
    class Attorney
    {
        static TextureManagerImpl* create() { return new TextureManagerImpl(); }
        static void destroy(TextureManager* texture_manager) { _destroy( texture_manager ); }
        friend class RenderingDeviceImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    TextureManagerImpl();

    /**
     * Destructor.
     */
    virtual ~TextureManagerImpl() = default;

private:
    /** Copy constructor. */
    TextureManagerImpl(const TextureManagerImpl&) = delete;
    /** Copy assignment operator. */
    TextureManagerImpl& operator = (const TextureManagerImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:



    // #############################################################################
    // Properties
    // #############################################################################
public:



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // TextureManager Implementations
    // #############################################################################
private:



    // #############################################################################
    // Helpers
    // #############################################################################
private:

}; // class TextureManagerImpl



} // namespace textures
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_TEXTUREMANAGERIMPL_H
