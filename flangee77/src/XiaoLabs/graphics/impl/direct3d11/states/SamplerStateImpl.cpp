#include "SamplerStateImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



    static D3D11_FILTER _d3d_filter_from(xl7::graphics::states::SamplerState::MinMagFilterType min_filter_type, xl7::graphics::states::SamplerState::MinMagFilterType mag_filter_type, xl7::graphics::states::SamplerState::MipFilterType mip_filter_type)
    {
        bool min_anisotropic = min_filter_type == xl7::graphics::states::SamplerState::MinMagFilterType::Anisotropic;
        bool mag_anisotropic = mag_filter_type == xl7::graphics::states::SamplerState::MinMagFilterType::Anisotropic;
        bool mip_anisotropic = mip_filter_type == xl7::graphics::states::SamplerState::MipFilterType::Anisotropic;
        bool all_anisotropic =
            min_anisotropic &&
            mag_anisotropic &&
            mip_anisotropic;

        if ( all_anisotropic )
            return D3D11_ENCODE_ANISOTROPIC_FILTER( D3D11_FILTER_REDUCTION_TYPE_STANDARD );

        if ( min_anisotropic ) min_filter_type = xl7::graphics::states::SamplerState::MinMagFilterType::Linear;
        if ( mag_anisotropic ) mag_filter_type = xl7::graphics::states::SamplerState::MinMagFilterType::Linear;
        if ( mip_anisotropic ) mip_filter_type = xl7::graphics::states::SamplerState::MipFilterType::Linear;

        // Mip-level sampling "None" to disable mipmapping is specifically covered elsewhere.
        // Direct3D 11 doesn't have a corresponding filter type, so we just take the "simplest" here: "Point".
        if ( mip_filter_type == xl7::graphics::states::SamplerState::MipFilterType::None )
            mip_filter_type = xl7::graphics::states::SamplerState::MipFilterType::Point;

        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MinMagFilterType::Point ) - 1 == static_cast<unsigned>( D3D11_FILTER_TYPE_POINT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MinMagFilterType::Linear ) - 1 == static_cast<unsigned>( D3D11_FILTER_TYPE_LINEAR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::Point ) - 1 == static_cast<unsigned>( D3D11_FILTER_TYPE_POINT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::Linear ) - 1 == static_cast<unsigned>( D3D11_FILTER_TYPE_LINEAR ) );

        return D3D11_ENCODE_BASIC_FILTER(
            static_cast<D3D11_FILTER_TYPE>( static_cast<unsigned>( min_filter_type ) - 1 ),
            static_cast<D3D11_FILTER_TYPE>( static_cast<unsigned>( mag_filter_type ) - 1 ),
            static_cast<D3D11_FILTER_TYPE>( static_cast<unsigned>( mip_filter_type ) - 1 ),
            D3D11_FILTER_REDUCTION_TYPE_STANDARD );
    }

    static D3D11_TEXTURE_ADDRESS_MODE _d3d_texture_address_mode_from(xl7::graphics::states::SamplerState::AddressMode address_mode)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Wrap ) == static_cast<unsigned>( D3D11_TEXTURE_ADDRESS_WRAP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Mirror ) == static_cast<unsigned>( D3D11_TEXTURE_ADDRESS_MIRROR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Clamp ) == static_cast<unsigned>( D3D11_TEXTURE_ADDRESS_CLAMP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Border ) == static_cast<unsigned>( D3D11_TEXTURE_ADDRESS_BORDER ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::MirrorOnce ) == static_cast<unsigned>( D3D11_TEXTURE_ADDRESS_MIRROR_ONCE ) );

        return static_cast<D3D11_TEXTURE_ADDRESS_MODE>( address_mode );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SamplerStateImpl::SamplerStateImpl(const CreateParams<Desc>& params)
        : SamplerState( params )
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
    bool SamplerStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        D3D11_SAMPLER_DESC sampler_desc;
        sampler_desc.Filter = _d3d_filter_from( _desc.min_filter_type, _desc.mag_filter_type, _desc.mip_filter_type );
        sampler_desc.AddressU = _d3d_texture_address_mode_from( _desc.address_u );
        sampler_desc.AddressV = _d3d_texture_address_mode_from( _desc.address_v );
        sampler_desc.AddressW = _d3d_texture_address_mode_from( _desc.address_w );
        sampler_desc.MipLODBias = _desc.lod_bias;
        sampler_desc.MaxAnisotropy = _desc.max_anisotropy;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampler_desc.BorderColor[0] = _desc.border_color.r;
        sampler_desc.BorderColor[1] = _desc.border_color.g;
        sampler_desc.BorderColor[2] = _desc.border_color.b;
        sampler_desc.BorderColor[3] = _desc.border_color.a;
        sampler_desc.MinLOD = _desc.min_lod;
        sampler_desc.MaxLOD = _desc.mip_filter_type == xl7::graphics::states::SamplerState::MipFilterType::None ? 0.0f : _desc.max_lod; // Is this sufficient, correct, and sensible?

        HRESULT hresult = d3d_device->CreateSamplerState(
            &sampler_desc,
            &_d3d_sampler_state );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateSamplerState") ) );
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool SamplerStateImpl::_dispose_impl()
    {
        _d3d_sampler_state.Reset();

        return true;
    }



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
