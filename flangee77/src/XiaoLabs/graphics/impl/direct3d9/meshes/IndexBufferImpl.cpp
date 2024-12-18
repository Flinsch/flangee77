#include "IndexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace meshes {



    static D3DFORMAT _d3d_format_from(xl7::graphics::meshes::IndexType index_type)
    {
        switch ( index_type )
        {
        case xl7::graphics::meshes::IndexType::UINT16:
            return D3DFMT_INDEX16;
        case xl7::graphics::meshes::IndexType::UINT32:
            return D3DFMT_INDEX32;
        default:
            assert( false );
        }

        return D3DFMT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    IndexBufferImpl::IndexBufferImpl(const CreateParams<Desc>& params)
        : IndexBuffer( params )
        , _d3d_format( _d3d_format_from( _desc.index_type ) )
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

        assert( get_data().empty() || get_data().size() == static_cast<size_t>( _size ) );

        HRESULT hresult = d3d_device->CreateIndexBuffer(
            _size,
            mappings::_d3d_usage_from( _desc.usage ),
            _d3d_format,
            mappings::_d3d_pool_from( _desc.usage ),
            &_d3d_index_buffer,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateIndexBuffer") ) );
            return false;
        }

        if ( get_data().empty() )
            return true;

        return _update_impl( data_provider, true, true );
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
        DWORD flags = 0;
        if ( discard )
            flags |= D3DLOCK_DISCARD;
        else if ( no_overwrite )
            flags |= D3DLOCK_NOOVERWRITE;

        void* dst;
        HRESULT hresult = _d3d_index_buffer->Lock(
            static_cast<unsigned>( data_provider.get_offset() ),
            static_cast<unsigned>( data_provider.get_size() ),
            &dst,
            flags );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DIndexBuffer9::Lock") ) );
            return false;
        }

        ::memcpy( dst, get_data().data() + data_provider.get_offset(), data_provider.get_size() );

        hresult = _d3d_index_buffer->Unlock();
        assert( SUCCEEDED(hresult) );

        return true;
    }



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
