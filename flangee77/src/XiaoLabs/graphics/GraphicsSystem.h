#ifndef XL7_GRAPHICS_GRAPHICSSYSTEM_H
#define XL7_GRAPHICS_GRAPHICSSYSTEM_H
#include "../DriverBasedComponent.h"

#include "./RenderingDevice.h"

#include <functional>



namespace xl7::graphics {



class GraphicsSystem
    : public DriverBasedComponent<GraphicsSystem>
{
    friend class cl7::creational::Singleton<GraphicsSystem>;

public:
    GraphicsSystem(const GraphicsSystem&) = delete;
    GraphicsSystem& operator=(const GraphicsSystem&) = delete;
    GraphicsSystem(GraphicsSystem&&) = delete;
    GraphicsSystem& operator=(GraphicsSystem&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the rendering device.
     */
    RenderingDevice* get_rendering_device() { return _rendering_device.get(); }

    /**
     * Returns the rendering device implementation.
     */
    template <class TRenderingDeviceImpl>
    TRenderingDeviceImpl* get_rendering_device_impl()
    {
        assert(static_cast<TRenderingDeviceImpl*>(get_rendering_device()) == dynamic_cast<TRenderingDeviceImpl*>(get_rendering_device()));
        return static_cast<TRenderingDeviceImpl*>(get_rendering_device());
    }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    GraphicsSystem();

    /**
     * Destructor.
     */
    ~GraphicsSystem() override = default;



    // #############################################################################
    // Factory Function
    // #############################################################################

    static GraphicsSystem* factory_func();



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterward.
     */
    virtual bool _init_before_rendering_device_impl() = 0;

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    virtual bool _shutdown_after_rendering_device_impl() = 0;

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterward.
     */
    virtual RenderingDevice* _rendering_device_factory_impl() = 0;



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool _init() final;

    /**
     * De-initializes the component.
     */
    bool _shutdown() final;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates and initializes the rendering device.
     */
    bool _create_rendering_device();

    /**
     * De-initializes and destroys the rendering device.
     */
    bool _destroy_rendering_device();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The rendering device.
     */
    std::unique_ptr<RenderingDevice, std::function<void(RenderingDevice*)>> _rendering_device;

}; // class GraphicsSystem



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_GRAPHICSSYSTEM_H
