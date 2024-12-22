#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
#include "../../../states/BlendState.h"

#include "../prerequisites.h"

#include "./D3DBlendStateTypeValues.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace states {



class BlendStateImpl final
    : public xl7::graphics::states::BlendState
{

public:
    class Attorney
    {
        static BlendStateImpl* create(const CreateParams<Desc>& params) { return new BlendStateImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    BlendStateImpl() = delete;

    BlendStateImpl(const BlendStateImpl&) = delete;
    BlendStateImpl& operator = (const BlendStateImpl&) = delete;
    BlendStateImpl(BlendStateImpl&&) = delete;
    BlendStateImpl& operator = (BlendStateImpl&&) = delete;



    /**
     * Returns the Direct3D 9 blend state type values.
     */
    const D3DBlendStateTypeValues& get_d3d_blend_state_type_values() const { return _d3d_blend_state_type_values; }

    /**
     * Maps the specified blend state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    static void map_d3d_values(const Desc& desc, D3DBlendStateTypeValues& d3d_blend_state_type_values);



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    BlendStateImpl(const CreateParams<Desc>& params);
    ~BlendStateImpl() override = default;



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
     * The Direct3D 9 blend state type values.
     */
    D3DBlendStateTypeValues _d3d_blend_state_type_values;

}; // class BlendStateImpl



} // namespace states
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
