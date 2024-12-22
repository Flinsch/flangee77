#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_DEPTHSTENCILSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_DEPTHSTENCILSTATEIMPL_H
#include "../../../states/DepthStencilState.h"

#include "../prerequisites.h"

#include "./D3DDepthStencilStateTypeValues.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace states {



class DepthStencilStateImpl final
    : public xl7::graphics::states::DepthStencilState
{

public:
    class Attorney
    {
        static DepthStencilStateImpl* create(const CreateParams<Desc>& params) { return new DepthStencilStateImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    DepthStencilStateImpl() = delete;

    DepthStencilStateImpl(const DepthStencilStateImpl&) = delete;
    DepthStencilStateImpl& operator = (const DepthStencilStateImpl&) = delete;
    DepthStencilStateImpl(DepthStencilStateImpl&&) = delete;
    DepthStencilStateImpl& operator = (DepthStencilStateImpl&&) = delete;



    /**
     * Returns the Direct3D 9 depth/stencil state type values.
     */
    const D3DDepthStencilStateTypeValues& get_d3d_depth_stencil_state_type_values() const { return _d3d_depth_stencil_state_type_values; }

    /**
     * Maps the specified depth/stencil state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    static void map_d3d_values(const Desc& desc, D3DDepthStencilStateTypeValues& d3d_depth_stencil_state_type_values);



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
    void* _get_raw_resource_impl() const override { return nullptr; }

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
     * The Direct3D 9 depth/stencil state type values.
     */
    D3DDepthStencilStateTypeValues _d3d_depth_stencil_state_type_values;

}; // class DepthStencilStateImpl



} // namespace states
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_DEPTHSTENCILSTATEIMPL_H
