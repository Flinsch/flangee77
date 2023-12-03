#pragma once
#ifndef XL7_GRAPHICS_RENDERINGDEVICE_H
#define XL7_GRAPHICS_RENDERINGDEVICE_H

#include "../ResourceManager.h"

#include <CoreLabs/Version.h>



namespace xl7 {
namespace graphics {



class RenderingDevice
{
    friend class GraphicsSystem;



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



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingDevice(std::unique_ptr<ResourceManager> resource_manager);

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
     * Placeholder for an example resource manager until the first actual resource
     * manager is implemented.
     */
    std::unique_ptr<ResourceManager> _resource_manager;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the capabilities of the rendering device (as far as they can be
     * determined).
     */
    const Capabilities& get_capabilities() const { return _capabilities; }



    // #############################################################################
    // Methods
    // #############################################################################
private:



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

}; // class RenderingDevice



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGDEVICE_H
