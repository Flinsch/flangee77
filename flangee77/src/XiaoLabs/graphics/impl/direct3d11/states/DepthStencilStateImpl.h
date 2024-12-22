#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
#include "../../../states/DepthStencilState.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {
    class ResourceFactoryImpl;
namespace states {



class DepthStencilStateImpl final
    : public xl7::graphics::states::DepthStencilState
{

public:
    class Attorney
    {
        static DepthStencilStateImpl* create(const CreateParams<Desc>& params) { return new DepthStencilStateImpl(params); }
        friend class xl7::graphics::impl::direct3d11::ResourceFactoryImpl;
    };



    DepthStencilStateImpl() = delete;

    DepthStencilStateImpl(const DepthStencilStateImpl&) = delete;
    DepthStencilStateImpl& operator=(const DepthStencilStateImpl&) = delete;
    DepthStencilStateImpl(DepthStencilStateImpl&&) = delete;
    DepthStencilStateImpl& operator=(DepthStencilStateImpl&&) = delete;



    /**
     * Returns the Direct3D 11 depth/stencil state interface.
     */
    ID3D11DepthStencilState* get_raw_d3d_depth_stencil_state() const { return _d3d_depth_stencil_state.Get(); }

    /**
     * Maps the specified depth/stencil state descriptor to corresponding Direct3D 11
     * values and fills the given structure accordingly.
     */
    static void map_d3d_values(const Desc& desc, D3D11_DEPTH_STENCIL_DESC& d3d_depth_stencil_desc);



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    DepthStencilStateImpl(const CreateParams<Desc>& params);
    ~DepthStencilStateImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_depth_stencil_state.Get(); }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const xl7::resources::DataProvider& data_provider) override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 11 depth/stencil state interface.
     */
    wrl::ComPtr<ID3D11DepthStencilState> _d3d_depth_stencil_state;

}; // class DepthStencilStateImpl



} // namespace states
} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
