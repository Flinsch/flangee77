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
        : _memory_info()
        , _resource_manager( std::move(resource_manager) )
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
        ::memset( &_memory_info, 0, sizeof(_memory_info) );

        return _init_impl( _memory_info );
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
