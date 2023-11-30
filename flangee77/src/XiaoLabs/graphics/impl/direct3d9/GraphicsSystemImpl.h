#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
#include "../../GraphicsSystem.h"

#include <d3d9.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



class GraphicsSystemImpl final
    : public GraphicsSystem
{
    friend GraphicsSystem* GraphicsSystem::factory_func();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    GraphicsSystemImpl() = default;

    /**
     * Destructor.
     */
    virtual ~GraphicsSystemImpl() override = default;

private:
    /** Copy constructor. */
    GraphicsSystemImpl(const GraphicsSystemImpl&) = delete;
    /** Copy assignment operator. */
    GraphicsSystemImpl& operator = (const GraphicsSystemImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 9 main interface.
     */
    wrl::ComPtr<IDirect3D9> _d3d_interface;



    // #############################################################################
    // Properties
    // #############################################################################
public:



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################
private:
    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterwards.
     */
    virtual bool _init_before_rendering_device_impl() override;

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    virtual bool _shutdown_after_rendering_device_impl() override;

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterwards.
     */
    virtual RenderingDevice* _rendering_device_factory_impl() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates the Direct3D 9 main interface.
     */
    bool _create_main_interface();

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool _release_main_interface();

}; // class GraphicsSystemImpl



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
