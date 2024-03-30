#include "RasterizerStateImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



    static D3D11_FILL_MODE _d3d_fill_mode_from(xl7::graphics::states::RasterizerState::FillMode fill_mode)
    {
        if ( fill_mode == xl7::graphics::states::RasterizerState::FillMode::None )
        {
            // Don't throw an error/warning message because this case is handled explicitly anyway.
            return D3D11_FILL_SOLID;
        }

        if ( fill_mode == xl7::graphics::states::RasterizerState::FillMode::Point )
        {
            LOG_WARNING( TEXT("\"Point\" fill mode is not supported.") );
            return D3D11_FILL_SOLID;
        }

        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::FillMode::Wireframe ) == static_cast<unsigned>( D3D11_FILL_WIREFRAME ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::FillMode::Solid ) == static_cast<unsigned>( D3D11_FILL_SOLID ) );

        return static_cast<D3D11_FILL_MODE>( fill_mode );
    }

    static D3D11_CULL_MODE _d3d_cull_mode_from(xl7::graphics::states::RasterizerState::CullMode cull_mode)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::CullMode::None ) + 1 == static_cast<unsigned>( D3D11_CULL_NONE ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::CullMode::Front ) + 1 == static_cast<unsigned>( D3D11_CULL_FRONT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::RasterizerState::CullMode::Back ) + 1 == static_cast<unsigned>( D3D11_CULL_BACK ) );

        return static_cast<D3D11_CULL_MODE>( static_cast<unsigned>( cull_mode ) + 1 );
    }

    static BOOL _d3d_front_counter_clockwise_from(xl7::graphics::states::RasterizerState::WindingOrder winding_order)
    {
        return winding_order == xl7::graphics::states::RasterizerState::WindingOrder::CounterClockwise ? TRUE : FALSE;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RasterizerStateImpl::RasterizerStateImpl(const CreateParams<Desc>& params)
        : RasterizerState( params )
    {
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
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        D3D11_RASTERIZER_DESC rasterizer_desc;
        rasterizer_desc.FillMode = _d3d_fill_mode_from( _desc.fill_mode );
        rasterizer_desc.CullMode = _d3d_cull_mode_from( _desc.cull_mode );
        rasterizer_desc.FrontCounterClockwise = _d3d_front_counter_clockwise_from( _desc.winding_order );
        rasterizer_desc.DepthBias = 0;
        rasterizer_desc.DepthBiasClamp = 0.0f;
        rasterizer_desc.SlopeScaledDepthBias = 0.0f;
        rasterizer_desc.DepthClipEnable = TRUE;
        rasterizer_desc.ScissorEnable = FALSE;
        rasterizer_desc.MultisampleEnable = FALSE;
        rasterizer_desc.AntialiasedLineEnable = FALSE;

        HRESULT hresult = d3d_device->CreateRasterizerState(
            &rasterizer_desc,
            &_d3d_rasterizer_state );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateRasterizerState") ) );
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool RasterizerStateImpl::_dispose_impl()
    {
        _d3d_rasterizer_state.Reset();

        return true;
    }



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
