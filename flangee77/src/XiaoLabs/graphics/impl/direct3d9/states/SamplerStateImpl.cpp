#include "SamplerStateImpl.h"

#include <MathLabs/utilities.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



    static D3DTEXTUREADDRESS _d3d_texture_address_from(xl7::graphics::states::SamplerState::AddressMode address_mode)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Wrap ) == static_cast<unsigned>( D3DTADDRESS_WRAP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Mirror ) == static_cast<unsigned>( D3DTADDRESS_MIRROR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Clamp ) == static_cast<unsigned>( D3DTADDRESS_CLAMP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::Border ) == static_cast<unsigned>( D3DTADDRESS_BORDER ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::AddressMode::MirrorOnce ) == static_cast<unsigned>( D3DTADDRESS_MIRRORONCE ) );

        return static_cast<D3DTEXTUREADDRESS>( address_mode );
    }

    static D3DTEXTUREFILTERTYPE _d3d_texture_filter_type_from(xl7::graphics::states::SamplerState::MinMagFilterType filter_type)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MinMagFilterType::Point ) == static_cast<unsigned>( D3DTEXF_POINT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MinMagFilterType::Linear ) == static_cast<unsigned>( D3DTEXF_LINEAR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MinMagFilterType::Anisotropic ) == static_cast<unsigned>( D3DTEXF_ANISOTROPIC ) );

        return static_cast<D3DTEXTUREFILTERTYPE>( filter_type );
    }

    static D3DTEXTUREFILTERTYPE _d3d_texture_filter_type_from(xl7::graphics::states::SamplerState::MipFilterType filter_type)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::None ) == static_cast<unsigned>( D3DTEXF_NONE ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::Point ) == static_cast<unsigned>( D3DTEXF_POINT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::Linear ) == static_cast<unsigned>( D3DTEXF_LINEAR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::SamplerState::MipFilterType::Anisotropic ) == static_cast<unsigned>( D3DTEXF_ANISOTROPIC ) );

        return static_cast<D3DTEXTUREFILTERTYPE>( filter_type );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SamplerStateImpl::SamplerStateImpl(const CreateParams<Desc>& params)
        : SamplerState( params )
        , _d3d_sampler_state_type_values()
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
        _d3d_sampler_state_type_values = D3DSamplerStateTypeValues( {
            { D3DSAMP_ADDRESSU, _d3d_texture_address_from( _desc.address_u ) },
            { D3DSAMP_ADDRESSV, _d3d_texture_address_from( _desc.address_v ) },
            { D3DSAMP_ADDRESSW, _d3d_texture_address_from( _desc.address_w ) },
            { D3DSAMP_BORDERCOLOR, _desc.border_color.to_uint32( xl7::graphics::ChannelOrder::BGRA ) },
            { D3DSAMP_MAGFILTER, _d3d_texture_filter_type_from( _desc.mag_filter_type ) },
            { D3DSAMP_MINFILTER, _d3d_texture_filter_type_from( _desc.min_filter_type ) },
            { D3DSAMP_MIPFILTER, _d3d_texture_filter_type_from( _desc.mip_filter_type ) },
            { D3DSAMP_MIPMAPLODBIAS, *reinterpret_cast<const DWORD*>( &_desc.lod_bias ) },
            { D3DSAMP_MAXMIPLEVEL, static_cast<DWORD>( ml7::utilities::round( _desc.max_lod ) ) },
            { D3DSAMP_MAXANISOTROPY, _desc.max_anisotropy },
            { D3DSAMP_SRGBTEXTURE, 0 },
            { D3DSAMP_ELEMENTINDEX, 0 },
            { D3DSAMP_DMAPOFFSET, 0 },
        } );

        return true;
    }

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool SamplerStateImpl::_release_impl()
    {
        return true;
    }



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
