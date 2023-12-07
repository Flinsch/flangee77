#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_TEXTUREMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_TEXTUREMANAGERIMPL_H
#include "../../TextureManager.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



class TextureManagerImpl final
    : public TextureManager
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



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_TEXTUREMANAGERIMPL_H
