#include "RenderingDevice.h"



namespace xl7 {
namespace graphics {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingDevice::RenderingDevice(std::unique_ptr<ResourceManager> resource_manager)
        : _resource_manager( std::move(resource_manager) )
    {
    }



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Initializes the rendering device.
     */
    bool RenderingDevice::_init()
    {
        return _init_impl();
    }

    /**
     * De-initializes the rendering device.
     */
    bool RenderingDevice::_shutdown()
    {
        return _shutdown_impl();
    }



} // namespace graphics
} // namespace xl7
