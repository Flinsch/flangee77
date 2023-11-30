#include "RenderingDeviceImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    RenderingDeviceImpl::RenderingDeviceImpl()
        : RenderingDevice( nullptr )
    {
    }



    // #############################################################################
    // RenderingDevice Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool RenderingDeviceImpl::_init_impl(MemoryInfo& memory_info)
    {
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool RenderingDeviceImpl::_shutdown_impl()
    {
        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
