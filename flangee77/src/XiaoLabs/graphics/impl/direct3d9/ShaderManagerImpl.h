#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERMANAGERIMPL_H
#include "../../ShaderManager.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



class ShaderManagerImpl final
    : public ShaderManager
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



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERMANAGERIMPL_H
