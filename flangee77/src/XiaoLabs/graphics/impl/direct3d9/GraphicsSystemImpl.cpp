#include "GraphicsSystemImpl.h"

#include "./RenderingDeviceImpl.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9 {



    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterward.
     */
    bool GraphicsSystemImpl::_init_before_rendering_device_impl()
    {
        if (!_create_main_interface())
            return false;

        LOG_TYPE(TEXT("Sorry for using \u201cold\u201d technology."), cl7::logging::LogType::Comment);

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
     * fully initializing it so that it can be initialized afterward.
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
        if (_d3d_main)
        {
            LOG_WARNING(TEXT("The Direct3D 9 main interface has already been created."));
            return true;
        }

        _d3d_main.Attach(::Direct3DCreate9(D3D_SDK_VERSION));

        if (!_d3d_main)
        {
            LOG_ERROR(TEXT("The Direct3D 9 main interface could not be created."));
            return false;
        }

        LOG(TEXT("The Direct3D 9 main interface has been created."));
        return true;
    }

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool GraphicsSystemImpl::_release_main_interface()
    {
        _d3d_main.Reset();

        LOG(TEXT("The Direct3D 9 main interface has been released."));
        return true;
    }



} // namespace xl7::graphics::impl::direct3d9
