#include "Texture2DImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture2DImpl::Texture2DImpl(const CreateParams<Desc>& params)
        : Texture2D( params )
        , _d3d_format( mappings::_d3d_format_from( params.desc.pixel_format, _channel_order ) )
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

        HRESULT hresult = d3d_device->CreateTexture(
            _desc.width,
            _desc.height,
            _desc.mip_levels,
            mappings::_d3d_usage_from( _desc.usage, _desc.mip_levels ),
            _d3d_format,
            mappings::_d3d_pool_from( _desc.usage ),
            &_d3d_texture,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateTexture") ) );
            return false;
        }

        auto pair = mappings::_map_d3d_format( _d3d_format, _desc.preferred_channel_order );
        assert( pair.first == _desc.pixel_format );
        assert( pair.second == _channel_order );

        if ( _data.empty() )
            return true;

        //return _update_impl( data_provider, true, true );
        return true;
    }



} // namespace textures
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
