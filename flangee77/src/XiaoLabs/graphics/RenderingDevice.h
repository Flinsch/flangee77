#pragma once
#ifndef XL7_GRAPHICS_RENDERINGDEVICE_H
#define XL7_GRAPHICS_RENDERINGDEVICE_H

#include "./surfaces/SurfaceManager.h"
#include "./textures/TextureManager.h"
#include "./meshes/MeshManager.h"
#include "./shaders/ShaderManager.h"

#include <CoreLabs/Version.h>



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
    RenderingDevice(
        SurfaceManagerPtr surface_manager,
        TextureManagerPtr texture_manager,
        MeshManagerPtr mesh_manager,
        ShaderManagerPtr shader_manager);

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

    /**
     * The flag indicating whether the function begin_scene has been called without
     * a following call to end_scene.
     */
    bool _the_scene_is_on;



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
     * Begins a scene.
     */
    bool begin_scene();

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool end_scene();

    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool present();



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
     * Begins a scene.
     */
    virtual bool _begin_scene_impl() = 0;

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    virtual bool _end_scene_impl() = 0;

    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    virtual bool _present_impl() = 0;

}; // class RenderingDevice



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGDEVICE_H
