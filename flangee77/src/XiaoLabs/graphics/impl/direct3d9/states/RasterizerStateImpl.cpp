#include "RasterizerStateImpl.h"



namespace xl7::graphics::impl::direct3d9::states {



namespace {

    D3DFILLMODE _d3d_fill_mode_from(graphics::states::FillMode fill_mode)
    {
        if (fill_mode == graphics::states::FillMode::None)
        {
            // Don't throw an error/warning message because this case is handled explicitly anyway.
            return D3DFILL_SOLID;
        }

        static_assert(static_cast<unsigned>(graphics::states::FillMode::Point) == static_cast<unsigned>(D3DFILL_POINT));
        static_assert(static_cast<unsigned>(graphics::states::FillMode::Wireframe) == static_cast<unsigned>(D3DFILL_WIREFRAME));
        static_assert(static_cast<unsigned>(graphics::states::FillMode::Solid) == static_cast<unsigned>(D3DFILL_SOLID));

        return static_cast<D3DFILLMODE>(fill_mode);
    }

    D3DCULL _d3d_cull_from(graphics::states::CullMode cull_mode, graphics::states::WindingOrder winding_order)
    {
        if (cull_mode == graphics::states::CullMode::None)
            return D3DCULL_NONE;

        if (cull_mode == graphics::states::CullMode::Front && winding_order == graphics::states::WindingOrder::Clockwise)
            return D3DCULL_CW;
        if (cull_mode == graphics::states::CullMode::Back && winding_order == graphics::states::WindingOrder::CounterClockwise)
            return D3DCULL_CW;

        return D3DCULL_CCW;
    }

} // namespace



    /**
     * Maps the specified rasterizer state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    void RasterizerStateImpl::map_d3d_values(const graphics::states::RasterizerStateDesc& desc, D3DRasterizerStateTypeValues& d3d_rasterizer_state_type_values)
    {
        d3d_rasterizer_state_type_values = D3DRasterizerStateTypeValues({
            {D3DRS_FILLMODE, _d3d_fill_mode_from(desc.fill_mode)},
            {D3DRS_CULLMODE, _d3d_cull_from(desc.cull_mode, desc.winding_order)},
        });
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RasterizerStateImpl::RasterizerStateImpl(const CreateContext& ctx, const graphics::states::RasterizerStateDesc& desc)
        : RasterizerState(ctx, desc)
        , _d3d_rasterizer_state_type_values()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool RasterizerStateImpl::_acquire_impl()
    {
        map_d3d_values(get_desc(), _d3d_rasterizer_state_type_values);

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool RasterizerStateImpl::_dispose_impl()
    {
        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::states
