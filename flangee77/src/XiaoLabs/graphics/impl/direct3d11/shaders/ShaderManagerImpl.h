#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_SHADERMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_SHADERMANAGERIMPL_H
#include "../../../shaders/ShaderManager.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class ShaderManagerImpl final
    : public xl7::graphics::shaders::ShaderManager
{

public:
    class Attorney
    {
        static ShaderManagerImpl* create() { return new ShaderManagerImpl(); }
        static void destroy(ShaderManager* shader_manager) { _destroy( shader_manager ); }
        friend class RenderingDeviceImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    ShaderManagerImpl();

    /**
     * Destructor.
     */
    virtual ~ShaderManagerImpl() = default;

private:
    /** Copy constructor. */
    ShaderManagerImpl(const ShaderManagerImpl&) = delete;
    /** Copy assignment operator. */
    ShaderManagerImpl& operator = (const ShaderManagerImpl&) = delete;



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
    // ShaderManager Implementations
    // #############################################################################
private:



    // #############################################################################
    // Helpers
    // #############################################################################
private:

}; // class ShaderManagerImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_SHADERMANAGERIMPL_H
