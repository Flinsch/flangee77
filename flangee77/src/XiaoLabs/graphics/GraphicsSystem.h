#pragma once
#ifndef XL7_GRAPHICS_GRAPHICSSYSTEM_H
#define XL7_GRAPHICS_GRAPHICSSYSTEM_H
#include "../Component.h"

#include "./RenderingDevice.h"

#include <functional>



#define XL7_GRAPHICS_IMPL_DIRECT3D9     0x6b8f8219
#define XL7_GRAPHICS_IMPL_DIRECT3D11    0x9b09a2b4

#ifndef XL7_GRAPHICS_IMPL_DEFAULT
#define XL7_GRAPHICS_IMPL_DEFAULT       XL7_GRAPHICS_IMPL_DIRECT3D11
#endif // !XL7_GRAPHICS_IMPL_DEFAULT

#ifndef XL7_GRAPHICS_IMPL
//#define XL7_GRAPHICS_IMPL               XL7_GRAPHICS_IMPL_DEFAULT
#define XL7_GRAPHICS_IMPL               XL7_GRAPHICS_IMPL_DIRECT3D9
#endif // !XL7_GRAPHICS_IMPL



namespace xl7 {
namespace graphics {



class GraphicsSystem
    : public Component<GraphicsSystem>
{
    friend cl7::creational::Singleton<GraphicsSystem>;



    // #############################################################################
    // Factory Function
    // #############################################################################
protected:
    static GraphicsSystem* factory_func();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    GraphicsSystem();

    /**
     * Destructor.
     */
    virtual ~GraphicsSystem() override = default;

private:
    /** Copy constructor. */
    GraphicsSystem(const GraphicsSystem&) = delete;
    /** Copy assignment operator. */
    GraphicsSystem& operator = (const GraphicsSystem&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The rendering device.
     */
    std::unique_ptr<RenderingDevice, std::function<void(RenderingDevice*)>> _rendering_device;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the rendering device.
     */
    RenderingDevice* get_rendering_device() { return _rendering_device.get(); }



    // #############################################################################
    // Component Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() final;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() final;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates and initializes the rendering device.
     */
    bool _create_rendering_device();

    /**
     * De-initializes and destroys the rendering device.
     */
    bool _destroy_rendering_device();



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterwards.
     */
    virtual bool _init_before_rendering_device_impl() = 0;

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    virtual bool _shutdown_after_rendering_device_impl() = 0;

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterwards.
     */
    virtual RenderingDevice* _rendering_device_factory_impl() = 0;

}; // class GraphicsSystem



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_GRAPHICSSYSTEM_H
