#include "RenderingDevice.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/memory.h>



namespace xl7 {
namespace graphics {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingDevice::RenderingDevice(std::unique_ptr<ResourceManager> resource_manager)
        : _capabilities()
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
        _capabilities = Capabilities();

        Capabilities capabilities;
        if ( !_init_impl( capabilities ) )
            return false;

        _capabilities = capabilities;

        // Print out the supported shader versions.
        LOG_TYPE( TEXT("Shader model versions:"), cl7::logging::LogType::Caption );
        LOG_TYPE( TEXT("Vertex shader\t") + _capabilities.shaders.vertex_shader_version.to_string( true ), cl7::logging::LogType::Item );
        LOG_TYPE( TEXT("Pixel shader\t") + _capabilities.shaders.pixel_shader_version.to_string( true ), cl7::logging::LogType::Item );

        // Print out the available video memory.
        LOG_TYPE( TEXT("Usable video memory:"), cl7::logging::LogType::Caption );
        LOG_TYPE( TEXT("Dedicated video memory\t") + cl7::memory::stringify_byte_amount( _capabilities.memory.dedicated_video_memory ), cl7::logging::LogType::Item );
        LOG_TYPE( TEXT("Dedicated system memory\t") + cl7::memory::stringify_byte_amount( _capabilities.memory.dedicated_system_memory ), cl7::logging::LogType::Item );
        LOG_TYPE( TEXT("Shared system memory\t") + cl7::memory::stringify_byte_amount( _capabilities.memory.shared_system_memory ), cl7::logging::LogType::Item );

        return true;
    }

    /**
     * De-initializes the rendering device.
     */
    bool RenderingDevice::_shutdown()
    {
        _capabilities = Capabilities();

        return _shutdown_impl();
    }



} // namespace graphics
} // namespace xl7
