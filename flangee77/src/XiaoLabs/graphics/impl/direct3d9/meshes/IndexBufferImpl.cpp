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



    static D3DFORMAT _d3d_format(xl7::graphics::meshes::IndexType index_type)
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
    IndexBufferImpl::IndexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : IndexBuffer( manager, identifier, desc )
        , d3d_format( _d3d_format( desc.index_type ) )
        , _d3d_device( dynamic_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_index_buffer()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool IndexBufferImpl::_request_impl()
    {
        return _create_index_buffer();
    }

    /**
     * Releases the resource.
     */
    bool IndexBufferImpl::_release_impl()
    {
        _d3d_index_buffer.Reset();

        return true;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    bool IndexBufferImpl::_resign_impl()
    {
        _d3d_index_buffer.Reset();

        return true;
    }

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    bool IndexBufferImpl::_restore_impl()
    {
        if ( !_create_index_buffer() )
            return false;

        // 

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the Direct3D 9 index buffer interface.
     */
    bool IndexBufferImpl::_create_index_buffer()
    {
        assert( _d3d_device );
        assert( !_d3d_index_buffer );

        HRESULT hresult = _d3d_device->CreateIndexBuffer(
            this->size,
            D3DUSAGE_WRITEONLY,
            this->d3d_format,
            D3DPOOL_MANAGED,
            &_d3d_index_buffer,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateIndexBuffer") ) );
            return false;
        }

        return true;
    }



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
