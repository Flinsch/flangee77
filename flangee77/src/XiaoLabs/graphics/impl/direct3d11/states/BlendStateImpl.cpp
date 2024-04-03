#include "BlendStateImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



    static D3D11_BLEND _d3d_blend_from(xl7::graphics::states::BlendState::BlendFactor blend_factor)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::Zero ) == static_cast<unsigned>( D3D11_BLEND_ZERO ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::One ) == static_cast<unsigned>( D3D11_BLEND_ONE ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::SrcColor ) == static_cast<unsigned>( D3D11_BLEND_SRC_COLOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::InvSrcColor ) == static_cast<unsigned>( D3D11_BLEND_INV_SRC_COLOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::SrcAlpha ) == static_cast<unsigned>( D3D11_BLEND_SRC_ALPHA ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::InvSrcAlpha ) == static_cast<unsigned>( D3D11_BLEND_INV_SRC_ALPHA ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::DestAlpha ) == static_cast<unsigned>( D3D11_BLEND_DEST_ALPHA ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::InvDestAlpha ) == static_cast<unsigned>( D3D11_BLEND_INV_DEST_ALPHA ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::DestColor ) == static_cast<unsigned>( D3D11_BLEND_DEST_COLOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::InvDestColor ) == static_cast<unsigned>( D3D11_BLEND_INV_DEST_COLOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::SrcAlphaSat ) == static_cast<unsigned>( D3D11_BLEND_SRC_ALPHA_SAT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::BlendFactor ) == static_cast<unsigned>( D3D11_BLEND_BLEND_FACTOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendFactor::InvBlendFactor ) == static_cast<unsigned>( D3D11_BLEND_INV_BLEND_FACTOR ) );

        return static_cast<D3D11_BLEND>( blend_factor );
    }

    static D3D11_BLEND_OP _d3d_blend_op_from(xl7::graphics::states::BlendState::BlendOperation blend_operation)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendOperation::Add ) == static_cast<unsigned>( D3D11_BLEND_OP_ADD ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendOperation::Subtract ) == static_cast<unsigned>( D3D11_BLEND_OP_SUBTRACT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendOperation::RevSubtract ) == static_cast<unsigned>( D3D11_BLEND_OP_REV_SUBTRACT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendOperation::Min ) == static_cast<unsigned>( D3D11_BLEND_OP_MIN ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::BlendState::BlendOperation::Max ) == static_cast<unsigned>( D3D11_BLEND_OP_MAX ) );

        return static_cast<D3D11_BLEND_OP>( blend_operation );
    }

    static UINT8 _d3d_render_target_write_mask_from(xl7::graphics::ChannelFlags channel_write_flags)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::ChannelFlags::Red ) == static_cast<unsigned>( D3D11_COLOR_WRITE_ENABLE_RED ) );
        static_assert( static_cast<unsigned>( xl7::graphics::ChannelFlags::Green ) == static_cast<unsigned>( D3D11_COLOR_WRITE_ENABLE_GREEN ) );
        static_assert( static_cast<unsigned>( xl7::graphics::ChannelFlags::Blue ) == static_cast<unsigned>( D3D11_COLOR_WRITE_ENABLE_BLUE ) );
        static_assert( static_cast<unsigned>( xl7::graphics::ChannelFlags::Alpha ) == static_cast<unsigned>( D3D11_COLOR_WRITE_ENABLE_ALPHA ) );

        return static_cast<UINT8>( channel_write_flags );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    BlendStateImpl::BlendStateImpl(const CreateParams<Desc>& params)
        : BlendState( params )
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
    bool BlendStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        D3D11_BLEND_DESC blend_desc;
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = _desc.is_blending_enabled ? TRUE : FALSE;
        blend_desc.RenderTarget[0].SrcBlend = _d3d_blend_from( _desc.src_color_factor );
        blend_desc.RenderTarget[0].DestBlend = _d3d_blend_from( _desc.dest_color_factor );
        blend_desc.RenderTarget[0].BlendOp = _d3d_blend_op_from( _desc.color_operation );
        blend_desc.RenderTarget[0].SrcBlendAlpha = _d3d_blend_from( _desc.src_alpha_factor );
        blend_desc.RenderTarget[0].DestBlendAlpha = _d3d_blend_from( _desc.dest_alpha_factor );
        blend_desc.RenderTarget[0].BlendOpAlpha = _d3d_blend_op_from( _desc.alpha_operation );
        blend_desc.RenderTarget[0].RenderTargetWriteMask = _d3d_render_target_write_mask_from( _desc.channel_write_flags );

        HRESULT hresult = d3d_device->CreateBlendState(
            &blend_desc,
            &_d3d_blend_state );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateBlendState") ) );
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool BlendStateImpl::_dispose_impl()
    {
        _d3d_blend_state.Reset();

        return true;
    }



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
