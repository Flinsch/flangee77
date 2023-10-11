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



} // namespace graphics
} // namespace xl7
