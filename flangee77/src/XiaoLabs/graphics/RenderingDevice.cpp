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
        , _device_lost( false )
        , _the_scene_is_on( false )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Checks whether the device is lost. If so, true is returned, and the
     * application should pause and periodically call handle_device_lost.
     * Reasons for a lost device could be:
     * * A full-screen application loses focus.
     * * The graphics driver is upgraded.
     * * The system changes from a power-saving adapter to a performance adapter.
     * * The graphics device stops responding and is reset.
     * * A graphics adapter is physically attached or removed.
     * * The swap chain has been resized.
     * * The app has been moved to a monitor attached to a difference adapter.
     * * The device has entered a non-operational state, for whatever reason.
     */
    bool RenderingDevice::check_device_lost()
    {
        // Already lost? Don't check again.
        if ( _device_lost )
            return true;

        if ( _check_device_lost_impl() )
            _notify_device_lost();
        return _device_lost;
    }

    /**
     * If the device is lost (see check_device_lost), the application should pause
     * and periodically call handle_device_lost to attempt to reset/reinitialize the
     * device and restore/reacquire/recreate the device-dependent resources. If the
     * device has been restored to an operational state, true is returned.
     */
    bool RenderingDevice::handle_device_lost()
    {
        // Not lost? // Nothing to do here.
        if ( !check_device_lost() )
            return true;

        const bool device_restored = _handle_device_lost_impl();
        _device_lost = !device_restored;
        return device_restored;
    }

    /**
     * Notifies about a "device lost" state.
     */
    void RenderingDevice::_notify_device_lost()
    {
        _device_lost = true;

        // Try to restore an operational state immediately?
        _handle_device_lost_impl();
    }

    /**
     * Begins a scene.
     */
    bool RenderingDevice::begin_scene()
    {
        if ( _the_scene_is_on )
        {
            LOG_WARNING( TEXT("The scene is already on.") );
            return false;
        }

        if ( !_begin_scene_impl() )
            return false;

        _the_scene_is_on = true;
        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingDevice::end_scene()
    {
        if ( !_the_scene_is_on )
        {
            LOG_WARNING( TEXT("The scene is not on.") );
            return false;
        }

        if ( !_end_scene_impl() )
            return false;

        _the_scene_is_on = false;
        return true;
    }

    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool RenderingDevice::present()
    {
        if ( _the_scene_is_on )
        {
            LOG_WARNING( TEXT("The scene is still on.") );
            return false;
        }

        if ( !_present_impl() )
            return false;

        return true;
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

        // Allriggedy, we got a new device
        // that surely is not lost.
        _device_lost = false;
        // And the scene is not on.
        _the_scene_is_on = false;

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
