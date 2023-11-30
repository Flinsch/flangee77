#include "GraphicsSystemImpl.h"

#include "./RenderingDeviceImpl.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>

#pragma comment( lib, "d3d9.lib" )



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterwards.
     */
    bool GraphicsSystemImpl::_init_before_rendering_device_impl()
    {
        if ( !_create_main_interface() )
            return false;

        return true;
    }

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    bool GraphicsSystemImpl::_shutdown_after_rendering_device_impl()
    {
        _release_main_interface();

        return true;
    }

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterwards.
     */
    RenderingDevice* GraphicsSystemImpl::_rendering_device_factory_impl()
    {
        return new RenderingDeviceImpl();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the Direct3D 9 main interface.
     */
    bool GraphicsSystemImpl::_create_main_interface()
    {
        if ( _d3d_interface )
        {
            LOG_WARNING( TEXT("The Direct3D 9 main interface has already been created.") );
            return true;
        }

        _d3d_interface.Attach( ::Direct3DCreate9( D3D_SDK_VERSION ) );

        if ( !_d3d_interface )
        {
            LOG_ERROR( TEXT("The Direct3D 9 main interface could not be created.") );
            return false;
        }

        LOG( TEXT("The Direct3D 9 main interface has been created.") );
        return true;
    }

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool GraphicsSystemImpl::_release_main_interface()
    {
        _d3d_interface.Reset();

        LOG( TEXT("The Direct3D 9 main interface has been released.") );
        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
