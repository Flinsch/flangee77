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
    RenderingDevice::RenderingDevice(std::unique_ptr<IResourceFactory> resource_factory)
        : _capabilities()
        , _resource_factory( std::move(resource_factory) )
        , _surface_manager( { surfaces::SurfaceManager::Attorney::create( _resource_factory.get() ), surfaces::SurfaceManager::Attorney::destroy } )
        , _texture_manager( { textures::TextureManager::Attorney::create( _resource_factory.get() ), textures::TextureManager::Attorney::destroy } )
        , _mesh_manager( { meshes::MeshManager::Attorney::create( _resource_factory.get() ), meshes::MeshManager::Attorney::destroy } )
        , _shader_manager( { shaders::ShaderManager::Attorney::create( _resource_factory.get() ), shaders::ShaderManager::Attorney::destroy } )
        , _device_lost( false )
    {
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the specified rendering context.
     */
    RenderingContext* RenderingDevice::get_rendering_context(unsigned index)
    {
        size_t i = static_cast<size_t>( index );
        if ( i < _rendering_contexts.size() )
            return _rendering_contexts[ i ].get();

        if ( i > _rendering_contexts.size() )
            LOG_WARNING( TEXT("The creation of a new rendering context was triggered whose index is out of sequence.") );
        while ( _rendering_contexts.size() < i )
            _rendering_contexts.push_back( nullptr );

        _rendering_contexts.emplace_back( _create_rendering_context_impl( index ), RenderingContext::Attorney::destroy );

        RenderingContext* rendering_context = _rendering_contexts[ i ].get();
        if ( rendering_context == nullptr )
        {
            if ( index == 0 )
                LOG_ERROR( TEXT("The primary rendering context could not be created.") );
            else
                LOG_WARNING( TEXT("An additional rendering context could not be created.") );
            return nullptr;
        }

        return rendering_context;
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
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool RenderingDevice::present()
    {
        bool the_scene_is_on = false;
        for ( const auto& rendering_context : _rendering_contexts )
            the_scene_is_on |= rendering_context->is_scene_on();

        if ( the_scene_is_on )
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

        if ( _capabilities.max_simultaneous_render_target_count > states::TargetStates::MAX_RENDER_TARGETS )
        {
            LOG_INFO( TEXT("Your rendering device seems capable of handling ") + cl7::to_string(_capabilities.max_simultaneous_render_target_count) + TEXT(" (color) render targets simultaneously, but this framework doesn't support more than ") + cl7::to_string(states::TargetStates::MAX_RENDER_TARGETS) + TEXT(" anyway.") );
            _capabilities.max_simultaneous_render_target_count = states::TargetStates::MAX_RENDER_TARGETS;
        }

        // Ensure (primary) rendering context.
        if ( get_rendering_context() == nullptr )
            return false;

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

        return true;
    }

    /**
     * De-initializes the rendering device.
     */
    bool RenderingDevice::_shutdown()
    {
        _rendering_contexts.clear();

        _capabilities = Capabilities();

        return _shutdown_impl();
    }



} // namespace graphics
} // namespace xl7
