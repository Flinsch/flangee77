#include "Texture2DImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture2DImpl::Texture2DImpl(const CreateParams<Desc>& params)
        : Texture2D( params )
        , _dxgi_format( mappings::_dxgi_format_from( params.desc.pixel_format, _channel_order ) )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool Texture2DImpl::_release_impl()
    {
        _d3d_shader_resource_view.Reset();
        _d3d_texture.Reset();

        return true;
    }



    // #############################################################################
    // Texture2D Implementations
    // #############################################################################

    /**
     * Requests/acquires a precompiled shader resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture2DImpl::_acquire_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider)
    {
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        assert( _data.empty() || _data.size() == static_cast<size_t>( _data_size ) );

        unsigned mip_levels = _desc.mip_levels;
        if ( _desc.usage == resources::ResourceUsage::Dynamic && mip_levels != 1 )
        {
            mip_levels = 1;
            LOG_WARNING( TEXT("A dynamic texture cannot have mipmaps, so the number of mipmap levels is set to 1.") );
        }

        D3D11_TEXTURE2D_DESC texture_desc;
        texture_desc.Width = _desc.width;
        texture_desc.Height = _desc.height;
        texture_desc.MipLevels = mip_levels;
        texture_desc.ArraySize = 1;
        texture_desc.Format = _dxgi_format;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = mappings::_d3d_usage_from( _desc.usage );
        texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags = _desc.usage == resources::ResourceUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        texture_desc.MiscFlags = 0;

        /*D3D11_SUBRESOURCE_DATA subresource_data;
        subresource_data.pSysMem = _data.data();
        subresource_data.SysMemPitch = _line_pitch;
        subresource_data.SysMemSlicePitch = 0;*/
        constexpr unsigned MAX_LEVELS = 16; // Just some value big enough. We could also specifically calculate the exact value, but not today.
        D3D11_SUBRESOURCE_DATA subresource_data[ MAX_LEVELS ];
        for ( unsigned i = 0, wdth = _desc.width, hght = _desc.height; wdth || hght; wdth >>= 1, hght >>= 1, ++i )
        {
            assert( i < MAX_LEVELS );
            subresource_data[ i ].pSysMem = _data.data(); // For now we simply set the same source buffer for all levels.
            subresource_data[ i ].SysMemPitch = (wdth ? wdth : 1) * _stride;
            subresource_data[ i ].SysMemSlicePitch = 0;
        }

        HRESULT hresult = d3d_device->CreateTexture2D(
            &texture_desc,
            _data.empty() ? nullptr : subresource_data,
            &_d3d_texture );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateTexture2D") ) );
            return false;
        }

        hresult = d3d_device->CreateShaderResourceView(
            _d3d_texture.Get(),
            nullptr,
            &_d3d_shader_resource_view );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateShaderResourceView") ) );
            return false;
        }

        auto pair = mappings::_map_dxgi_format( _dxgi_format, _desc.preferred_channel_order );
        assert( pair.first == _desc.pixel_format );
        assert( pair.second == _channel_order );

        return true;
    }



} // namespace textures
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
