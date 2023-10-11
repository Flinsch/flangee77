#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
#include "../../GraphicsSystem.h"

#include <dxgi.h>
#include <d3d11.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



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
     * The DXGI factory interface.
     */
    wrl::ComPtr<IDXGIFactory> _dxgi_factory;



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
    virtual bool _init_before_rendering_device() override;

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    virtual bool _shutdown_after_rendering_device() override;

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterwards.
     */
    virtual RenderingDevice* _rendering_device_factory() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates the DXGI factory interface.
     */
    bool _create_dxgi_factory();

    /**
     * Releases the DXGI factory interface.
     */
    bool _release_dxgi_factory();

}; // class GraphicsSystemImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
