#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_RASTERIZERSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_RASTERIZERSTATEIMPL_H
#include "../../../states/RasterizerState.h"

#include "../prerequisites.h"

#include "./D3DRasterizerStateTypeValues.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace states {



class RasterizerStateImpl final
    : public xl7::graphics::states::RasterizerState
{

public:
    class Attorney
    {
        static RasterizerStateImpl* create(const CreateParams<Desc>& params) { return new RasterizerStateImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    RasterizerStateImpl() = delete;

    RasterizerStateImpl(const RasterizerStateImpl&) = delete;
    RasterizerStateImpl& operator=(const RasterizerStateImpl&) = delete;
    RasterizerStateImpl(RasterizerStateImpl&&) = delete;
    RasterizerStateImpl& operator=(RasterizerStateImpl&&) = delete;



    /**
     * Returns the Direct3D 9 rasterizer state type values.
     */
    const D3DRasterizerStateTypeValues& get_d3d_rasterizer_state_type_values() const { return _d3d_rasterizer_state_type_values; }

    /**
     * Maps the specified rasterizer state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    static void map_d3d_values(const Desc& desc, D3DRasterizerStateTypeValues& d3d_rasterizer_state_type_values);



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    explicit RasterizerStateImpl(const CreateParams<Desc>& params);
    ~RasterizerStateImpl() override = default;



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
     * The Direct3D 9 rasterizer state type values.
     */
    D3DRasterizerStateTypeValues _d3d_rasterizer_state_type_values;

}; // class RasterizerStateImpl



} // namespace states
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_RASTERIZERSTATEIMPL_H
