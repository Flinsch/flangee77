#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SURFACES_SURFACEMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SURFACES_SURFACEMANAGERIMPL_H
#include "../../../surfaces/SurfaceManager.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace surfaces {



class SurfaceManagerImpl final
    : public xl7::graphics::surfaces::SurfaceManager
{

public:
    class Attorney
    {
        static SurfaceManagerImpl* create() { return new SurfaceManagerImpl(); }
        static void destroy(SurfaceManager* surface_manager) { _destroy( surface_manager ); }
        friend class RenderingDeviceImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    SurfaceManagerImpl();

    /**
     * Destructor.
     */
    virtual ~SurfaceManagerImpl() = default;

private:
    /** Copy constructor. */
    SurfaceManagerImpl(const SurfaceManagerImpl&) = delete;
    /** Copy assignment operator. */
    SurfaceManagerImpl& operator = (const SurfaceManagerImpl&) = delete;



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
    // SurfaceManager Implementations
    // #############################################################################
private:



    // #############################################################################
    // Helpers
    // #############################################################################
private:

}; // class SurfaceManagerImpl



} // namespace surfaces
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SURFACES_SURFACEMANAGERIMPL_H
