#include "GraphicsSystem.h"

#if XL7_GRAPHICS_IMPL == XL7_GRAPHICS_IMPL_DIRECT3D9
#define XL7_GRAPHICS_IMPL_NAME "Direct3D 9"
#include "./impl/direct3d9/GraphicsSystemImpl.h"
typedef xl7::graphics::impl::direct3d9::GraphicsSystemImpl GraphicsSystemImpl;

#elif XL7_GRAPHICS_IMPL == XL7_GRAPHICS_IMPL_DIRECT3D11
#define XL7_GRAPHICS_IMPL_NAME "Direct3D 11"
#include "./impl/direct3d11/GraphicsSystemImpl.h"
typedef xl7::graphics::impl::direct3d11::GraphicsSystemImpl GraphicsSystemImpl;

#else
#ifdef XL7_GRAPHICS_IMPL
#error Bad XL7_GRAPHICS_IMPL definition.
#else
#error Missing XL7_GRAPHICS_IMPL definition.
#endif
#endif

#pragma message( "The XiaoLabs graphics implementation is based on " XL7_GRAPHICS_IMPL_NAME "." )

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {



    // #############################################################################
    // Factory Function
    // #############################################################################

    GraphicsSystem* GraphicsSystem::factory_func()
    {
        return new GraphicsSystemImpl();
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    GraphicsSystem::GraphicsSystem()
        : _rendering_device( nullptr, RenderingDevice::Attorney::destroy )
    {
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool GraphicsSystem::_init()
    {
        const bool result = _init_before_rendering_device_impl() && _create_rendering_device();

        if ( result )
            LOG_SUCCESS( TEXT("The graphics component based on " XL7_GRAPHICS_IMPL_NAME " has been successfully initialized.") );
        else
            LOG_ERROR( TEXT("The graphics component based on " XL7_GRAPHICS_IMPL_NAME " could not be initialized.") );

        return result;
    }

    /**
     * De-initializes the component.
     */
    bool GraphicsSystem::_shutdown()
    {
        bool result = _destroy_rendering_device();
        if ( !_shutdown_after_rendering_device_impl() )
            result = false;

        if ( result )
            LOG_SUCCESS( TEXT("The graphics component based on " XL7_GRAPHICS_IMPL_NAME " has been shut down successfully.") );
        else
            LOG_WARNING( TEXT("The graphics component based on " XL7_GRAPHICS_IMPL_NAME " could not be shut down correctly.") );

        return result;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates and initializes the rendering device.
     */
    bool GraphicsSystem::_create_rendering_device()
    {
        _rendering_device.reset( _rendering_device_factory_impl() );
        if ( !_rendering_device )
        {
            LOG_ERROR( TEXT("The rendering device could not be created.") );
            return false;
        }

        if ( !RenderingDevice::Attorney::init( _rendering_device.get() ) )
        {
            LOG_ERROR( TEXT("The rendering device could not be initialized.") );
            return false;
        }

        LOG( TEXT("The rendering device has been created and initialized.") );

        return true;
    }

    /**
     * De-initializes and destroys the rendering device.
     */
    bool GraphicsSystem::_destroy_rendering_device()
    {
        if ( !_rendering_device )
        {
            LOG_WARNING( TEXT("There is no rendering device to destroy.") );
            return false;
        }

        if ( !RenderingDevice::Attorney::shutdown( _rendering_device.get() ) )
            LOG_WARNING( TEXT("The rendering device could not be shut down correctly.") );
        _rendering_device.reset();
        LOG( TEXT("The rendering device has been destroyed.") );

        return true;
    }



} // namespace graphics
} // namespace xl7
