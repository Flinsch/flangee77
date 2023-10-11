#pragma once
#ifndef XL7_GRAPHICS_RENDERINGDEVICE_H
#define XL7_GRAPHICS_RENDERINGDEVICE_H

#include "../ResourceManager.h"



namespace xl7 {
namespace graphics {



class GraphicsSystem;



class RenderingDevice
{
    friend GraphicsSystem;



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
     * Placeholder for an example resource manager until the first actual resource
     * manager is implemented.
     */
    std::unique_ptr<ResourceManager> _resource_manager;



    // #############################################################################
    // Properties
    // #############################################################################
private:



    // #############################################################################
    // Methods
    // #############################################################################
private:



    // #############################################################################
    // Lifetime Management
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() = 0;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() = 0;

}; // class RenderingDevice



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGDEVICE_H
