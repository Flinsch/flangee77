#ifndef XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
#include "../../GraphicsSystem.h"

#include "./prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {



class GraphicsSystemImpl final
    : public GraphicsSystem
{
    friend GraphicsSystem* GraphicsSystem::factory_func();

public:
    GraphicsSystemImpl(const GraphicsSystemImpl&) = delete;
    GraphicsSystemImpl& operator=(const GraphicsSystemImpl&) = delete;
    GraphicsSystemImpl(GraphicsSystemImpl&&) = delete;
    GraphicsSystemImpl& operator=(GraphicsSystemImpl&&) = delete;



    /**
     * Returns the DXGI factory interface.
     */
    IDXGIFactoryN* get_raw_dxgi_factory() const { return _dxgi_factory.Get(); }



protected:
    GraphicsSystemImpl() = default;
    ~GraphicsSystemImpl() override = default;



private:

    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterward.
     */
    bool _init_before_rendering_device_impl() override;

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    bool _shutdown_after_rendering_device_impl() override;

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterward.
     */
    RenderingDevice* _rendering_device_factory_impl() override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the DXGI factory interface.
     */
    bool _create_dxgi_factory();

    /**
     * Releases the DXGI factory interface.
     */
    bool _release_dxgi_factory();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The DXGI factory interface.
     */
    wrl::ComPtr<IDXGIFactoryN> _dxgi_factory;

}; // class GraphicsSystemImpl



} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_GRAPHICSSYSTEMIMPL_H
