#include "IndexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../RenderingContextImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace meshes {



    static DXGI_FORMAT _dxgi_format_from(xl7::graphics::meshes::IndexType index_type)
    {
        switch ( index_type )
        {
        case xl7::graphics::meshes::IndexType::UINT16:
            return DXGI_FORMAT_R16_UINT;
        case xl7::graphics::meshes::IndexType::UINT32:
            return DXGI_FORMAT_R32_UINT;
        default:
            assert( false );
        }

        return DXGI_FORMAT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    IndexBufferImpl::IndexBufferImpl(const CreateParams<Desc>& params)
        : IndexBuffer( params )
        , _dxgi_format( _dxgi_format_from( get_desc().index_type ) )
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
    bool IndexBufferImpl::_acquire_impl(const resources::DataProvider& data_provider)
    {
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        assert( get_data().empty() || get_data().size() == static_cast<size_t>( get_size() ) );

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = get_size();
        buffer_desc.Usage = mappings::_d3d_usage_from( get_desc().usage );
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.CPUAccessFlags = get_desc().usage == resources::ResourceUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = get_stride();

        D3D11_SUBRESOURCE_DATA subresource_data;
        subresource_data.pSysMem = get_data().data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        HRESULT hresult = d3d_device->CreateBuffer(
            &buffer_desc,
            get_data().empty() ? nullptr : &subresource_data,
            &_d3d_index_buffer );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateBuffer") ) );
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool IndexBufferImpl::_dispose_impl()
    {
        _d3d_index_buffer.Reset();

        return true;
    }



    // #############################################################################
    // IndexBuffer Implementations
    // #############################################################################

    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool IndexBufferImpl::_update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite)
    {
        auto d3d_device_context = static_cast<RenderingContextImpl*>( GraphicsSystem::instance().get_rendering_device()->get_primary_context() )->get_raw_d3d_device_context();
        assert( d3d_device_context );

        bool entire = data_provider.get_offset() == 0 && data_provider.get_size() == static_cast<size_t>( get_size() );

        if ( get_desc().usage == resources::ResourceUsage::Dynamic )
        {
            D3D11_MAP map_type;
            if ( discard )
                map_type = D3D11_MAP_WRITE_DISCARD;
            else if ( no_overwrite )
                map_type = D3D11_MAP_WRITE_NO_OVERWRITE;
            else
                map_type = D3D11_MAP_WRITE;

            D3D11_MAPPED_SUBRESOURCE mapped_subresource;
            HRESULT hresult = d3d_device_context->Map( _d3d_index_buffer.Get(), 0, map_type, 0, &mapped_subresource );

            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11DeviceContext::Map") ) );
                LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be mapped for writing.") );
                return false;
            }

            ::memcpy( mapped_subresource.pData, get_data().data() + data_provider.get_offset(), data_provider.get_size() );

            d3d_device_context->Unmap( _d3d_index_buffer.Get(), 0 );
        }
        else // => _desc.usage == ResourceUsage::Default
        {
            unsigned copy_flags = 0;
            if ( discard )
                copy_flags |= D3D11_COPY_DISCARD;
            else if ( no_overwrite )
                copy_flags |= D3D11_COPY_NO_OVERWRITE;

            D3D11_BOX box;
            box.left = static_cast<unsigned>( data_provider.get_offset() );
            box.top = 0;
            box.front = 0;
            box.right = static_cast<unsigned>( data_provider.get_offset() + data_provider.get_size() );
            box.bottom = 1;
            box.back = 1;

            d3d_device_context->UpdateSubresource1( _d3d_index_buffer.Get(), 0, &box, get_data().data() + data_provider.get_offset(), 0, 0, copy_flags );
        }

        return true;
    }



} // namespace meshes
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
