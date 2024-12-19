#include "RasterizerStateImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



    static D3DFILLMODE _d3d_fill_mode_from(xl7::graphics::states::RasterizerState::FillMode fill_mode)
    {
        if ( fill_mode == xl7::graphics::states::RasterizerState::FillMode::None )
        {
            // Don't throw an error/warning message because this case is handled explicitly anyway.
            return D3DFILL_SOLID;
        }

        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::FillMode::Point ) == static_cast<unsigned>( D3DFILL_POINT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::FillMode::Wireframe ) == static_cast<unsigned>( D3DFILL_WIREFRAME ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::FillMode::Solid ) == static_cast<unsigned>( D3DFILL_SOLID ) );

        return static_cast<D3DFILLMODE>( fill_mode );
    }

    static D3DCULL _d3d_cull_from(xl7::graphics::states::RasterizerState::CullMode cull_mode, xl7::graphics::states::RasterizerState::WindingOrder winding_order)
    {
        if ( cull_mode == xl7::graphics::states::RasterizerState::CullMode::None )
            return D3DCULL_NONE;

        if ( cull_mode == xl7::graphics::states::RasterizerState::CullMode::Front && winding_order == xl7::graphics::states::RasterizerState::WindingOrder::Clockwise )
            return D3DCULL_CW;
        if ( cull_mode == xl7::graphics::states::RasterizerState::CullMode::Back && winding_order == xl7::graphics::states::RasterizerState::WindingOrder::CounterClockwise )
            return D3DCULL_CW;

        return D3DCULL_CCW;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RasterizerStateImpl::RasterizerStateImpl(const CreateParams<Desc>& params)
        : RasterizerState( params )
        , _d3d_rasterizer_state_type_values()
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Maps the specified rasterizer state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    void RasterizerStateImpl::map_d3d_values(const Desc& desc, D3DRasterizerStateTypeValues& d3d_rasterizer_state_type_values)
    {
        d3d_rasterizer_state_type_values = D3DRasterizerStateTypeValues( {
            { D3DRS_FILLMODE, _d3d_fill_mode_from( desc.fill_mode ) },
            { D3DRS_CULLMODE, _d3d_cull_from( desc.cull_mode, desc.winding_order ) },
        } );
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool RasterizerStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        map_d3d_values( get_desc(), _d3d_rasterizer_state_type_values );

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool RasterizerStateImpl::_dispose_impl()
    {
        return true;
    }



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
