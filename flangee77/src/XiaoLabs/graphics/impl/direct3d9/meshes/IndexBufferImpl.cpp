#include "IndexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
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
        , _d3d_device( dynamic_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_index_buffer()
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
    bool IndexBufferImpl::_acquire_impl(const DataProvider& data_provider)
    {
        assert( _d3d_device );

        assert( _data.size() == static_cast<size_t>( _size ) );

        HRESULT hresult = _d3d_device->CreateIndexBuffer(
            _size,
            D3DUSAGE_WRITEONLY,
            _d3d_format,
            D3DPOOL_MANAGED,
            &_d3d_index_buffer,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateIndexBuffer") ) );
            return false;
        }

        void* dst;
        hresult = _d3d_index_buffer->Lock(
            0,
            _size,
            &dst,
            D3DLOCK_DISCARD );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DIndexBuffer9::Lock") ) );
            return false;
        }

        ::memcpy( dst, _data.data(), _data.size() );

        hresult = _d3d_index_buffer->Unlock();
        assert( SUCCEEDED(hresult) );

        return true;
    }

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool IndexBufferImpl::_release_impl()
    {
        _d3d_index_buffer.Reset();

        return true;
    }



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
