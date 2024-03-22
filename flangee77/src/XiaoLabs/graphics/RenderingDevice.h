#pragma once
#ifndef XL7_GRAPHICS_RENDERINGDEVICE_H
#define XL7_GRAPHICS_RENDERINGDEVICE_H

#include "./RenderingContext.h"

#include "./IResourceFactory.h"

#include "./surfaces/SurfaceManager.h"
#include "./textures/TextureManager.h"
#include "./meshes/MeshManager.h"
#include "./shaders/ShaderManager.h"

#include <CoreLabs/Version.h>

#include <vector>



namespace xl7 {
namespace graphics {



class RenderingDevice
{

public:
    class Attorney
    {
        static void destroy(RenderingDevice* rendering_device) { delete rendering_device; }
        static bool init(RenderingDevice* rendering_device) { return rendering_device->_init(); }
        static bool shutdown(RenderingDevice* rendering_device) { return rendering_device->_shutdown(); }
        friend class GraphicsSystem;
    };



public:
    struct Capabilities
    {
        struct Memory
        {
            /** The number of bytes of dedicated video memory that are not shared with the CPU. */
            size_t dedicated_video_memory = 0;
            /** The number of bytes of dedicated system memory that are not shared with the CPU. This memory is allocated from available system memory at boot time. */
            size_t dedicated_system_memory = 0;
            /** The number of bytes of shared system memory. This is the maximum value of system memory that may be consumed by the adapter during operation. Any incidental memory consumed by the driver as it manages and uses video memory is additional. */
            size_t shared_system_memory = 0;
        } memory;

        /** The maximum number of simultaneous (color) render targets. */
        unsigned max_simultaneous_render_target_count;
        /** The maximum number of concurrent vertex data streams. */
        unsigned max_concurrent_vertex_stream_count;

        struct
        {
            /** The maximum width of 2D textures, in pixels. */
            unsigned max_texture_2d_width;
            /** The maximum height of 2D textures, in pixels. */
            unsigned max_texture_2d_height;
            /** The maximum width, height, and depth of 3D textures, in pixels. */
            unsigned max_texture_3d_size;
            /** The maximum width and height of cubemaps, in pixels. */
            unsigned max_cubemap_size;
            /** The maximum number of textures in a texture array. */
            unsigned max_texture_array_size;

            /** The maximum texture aspect ratio (0: no limit). */
            unsigned max_aspect_ratio;

            /** The flag that indicates whether 2D textures must have dimensions specified as powers of two. */
            bool texture_2d_pow2_only;
            /** The flag that indicates whether 3D textures must have dimensions specified as powers of two. */
            bool texture_3d_pow2_only;
            /** The flag that indicates whether cubemaps must have dimensions specified as powers of two. */
            bool cubemap_pow2_only;
            /** The flag that indicates whether all textures must be square. */
            bool square_only;
        } textures;

        struct
        {
            /** The maximum supported vertex shader version. */
            cl7::Version vertex_shader_version;
            /** The maximum supported pixel shader version. */
            cl7::Version pixel_shader_version;
        } shaders;

    }; // struct Capabilities



protected:
    template <class TResourceManager>
    using ResourceManagerPtr = std::unique_ptr<TResourceManager, std::function<void(TResourceManager*)>>;

    typedef ResourceManagerPtr<surfaces::SurfaceManager> SurfaceManagerPtr;
    typedef ResourceManagerPtr<textures::TextureManager> TextureManagerPtr;
    typedef ResourceManagerPtr<meshes::MeshManager> MeshManagerPtr;
    typedef ResourceManagerPtr<shaders::ShaderManager> ShaderManagerPtr;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingDevice(std::unique_ptr<IResourceFactory> resource_factory);

    /**
     * Destructor.
     */
    virtual ~RenderingDevice() = default;

private:
    /** Default constructor. */
    RenderingDevice() = delete;
    /** Copy constructor. */
    RenderingDevice(const RenderingDevice&) = delete;
    /** Copy assignment operator. */
    RenderingDevice& operator = (const RenderingDevice&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The capabilities of the rendering device (as far as they can be determined).
     */
    Capabilities _capabilities;

private:
    /**
     * The rendering contexts.
     */
    std::vector<std::unique_ptr<RenderingContext, std::function<void(RenderingContext*)>>> _rendering_contexts;

private:
    /**
     * The central resource factory for the resource managers.
     */
    std::unique_ptr<IResourceFactory> _resource_factory;

    /**
     * The surface manager.
     */
    SurfaceManagerPtr _surface_manager;

    /**
     * The texture manager.
     */
    TextureManagerPtr _texture_manager;

    /**
     * The mesh manager.
     */
    MeshManagerPtr _mesh_manager;

    /**
     * The shader manager.
     */
    ShaderManagerPtr _shader_manager;

private:
    /**
     * The flag indicating whether the device was lost.
     */
    bool _device_lost;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the capabilities of the rendering device (as far as they can be
     * determined).
     */
    const Capabilities& get_capabilities() const { return _capabilities; }

public:
    /**
     * Returns the primary rendering context.
     */
    RenderingContext* get_primary_context() const { return _rendering_contexts[ 0 ].get(); }

    /**
     * Returns the specified rendering context (0: primary context).
     */
    RenderingContext* get_rendering_context(unsigned index = 0);

public:
    /**
     * Returns the surface manager.
     */
    surfaces::SurfaceManager* get_surface_manager() const { return _surface_manager.get(); }

    /**
     * Returns the texture manager.
     */
    textures::TextureManager* get_texture_manager() const { return _texture_manager.get(); }

    /**
     * Returns the mesh manager.
     */
    meshes::MeshManager* get_mesh_manager() const { return _mesh_manager.get(); }

    /**
     *  Returns the shader manager.
     */
    shaders::ShaderManager* get_shader_manager() const { return _shader_manager.get(); }



    // #############################################################################
    // Methods
    // #############################################################################
public:
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
    bool check_device_lost();

    /**
     * If the device is lost (see check_device_lost), the application should pause
     * and periodically call handle_device_lost to attempt to reset/reinitialize the
     * device and restore/reacquire/recreate the device-dependent resources. If the
     * device has been restored to an operational state, true is returned.
     */
    bool handle_device_lost();

protected:
    /**
     * Notifies about a "device lost" state.
     */
    void _notify_device_lost();

public:
    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool present();

public:
    /**
     * Checks whether the device (generally) supports the specified combination of
     * pixel format and channel order for the specified texture type.
     */
    bool check_texture_format(textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder channel_order);

    /**
     * Returns the channel order that is most likely to be accepted by the device
     * for the specified texture type of a specific pixel format. However, there is
     * no guarantee that the implied combination will actually be accepted (e.g., if
     * the pixel format is not supported at all). The secondary return value may
     * provide information about this.
     */
    std::pair<ChannelOrder, bool> recommend_channel_order(textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder preferred_channel_order);



    // #############################################################################
    // Lifetime Management
    // #############################################################################
private:
    /**
     * Initializes the rendering device.
     */
    bool _init();

    /**
     * De-initializes the rendering device.
     */
    bool _shutdown();



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Initializes the rendering device and determines the capabilities (as far as
     * they can be determined).
     */
    virtual bool _init_impl(Capabilities& capabilities) = 0;

    /**
     * De-initializes the rendering device.
     */
    virtual bool _shutdown_impl() = 0;

private:
    /**
     * Creates a new rendering context with the specified index (0: primary context).
     * Returns NULL if the rendering context could not be created.
     */
    virtual RenderingContext* _create_rendering_context_impl(unsigned index) = 0;

private:
    /**
     * Checks whether the device is lost. If so, true is returned.
     */
    virtual bool _check_device_lost_impl() = 0;

    /**
     * Tries to reset/reinitialize the device after it has been lost. If the device
     * has been restored to an operational state, true is returned.
     */
    virtual bool _handle_device_lost_impl() = 0;

private:
    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    virtual bool _present_impl() = 0;

private:
    /**
     * Checks whether the device (generally) supports the specified combination of
     * pixel format and channel order for the specified texture type.
     */
    virtual bool _check_texture_format_impl(textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder channel_order) = 0;

}; // class RenderingDevice



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGDEVICE_H
