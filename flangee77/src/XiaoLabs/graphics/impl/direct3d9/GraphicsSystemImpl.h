#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
#include "../../GraphicsSystem.h"

#include "./prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {



class GraphicsSystemImpl final
    : public GraphicsSystem
{
    friend GraphicsSystem* GraphicsSystem::factory_func();

public:
    GraphicsSystemImpl(const GraphicsSystemImpl&) = delete;
    GraphicsSystemImpl& operator = (const GraphicsSystemImpl&) = delete;
    GraphicsSystemImpl(GraphicsSystemImpl&&) = delete;
    GraphicsSystemImpl& operator = (GraphicsSystemImpl&&) = delete;



    /**
     * Returns the Direct3D 9 main interface.
     */
    IDirect3D9* get_raw_d3d_main() const { return _d3d_main.Get(); }



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
     * Creates the Direct3D 9 main interface.
     */
    bool _create_main_interface();

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool _release_main_interface();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 9 main interface.
     */
    wrl::ComPtr<IDirect3D9> _d3d_main;

}; // class GraphicsSystemImpl



} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_GRAPHICSSYSTEMIMPL_H
