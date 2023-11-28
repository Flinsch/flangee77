#include "RenderingDeviceImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



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
    // Lifetime Management
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool RenderingDeviceImpl::_init()
    {
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool RenderingDeviceImpl::_shutdown()
    {
        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
