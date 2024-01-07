#include "VertexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace meshes {



    static DWORD _d3d_fvf_from(const xl7::graphics::meshes::VertexLayout& vertex_layout)
    {
        

        return D3DFMT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexBufferImpl::VertexBufferImpl(const CreateParams<Desc>& params)
        : VertexBuffer( params )
        , _d3d_fvf( _d3d_fvf_from( _desc.vertex_layout ) )
        , _d3d_device( dynamic_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_vertex_buffer()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool VertexBufferImpl::_acquire_impl()
    {
        assert( _d3d_device );
        assert( !_d3d_vertex_buffer );

        HRESULT hresult = _d3d_device->CreateVertexBuffer(
            _size,
            D3DUSAGE_WRITEONLY,
            _d3d_fvf,
            D3DPOOL_MANAGED,
            &_d3d_vertex_buffer,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateVertexBuffer") ) );
            return false;
        }

        return true;
    }

    /**
     * Releases/"unacquires" the resource.
     */
    bool VertexBufferImpl::_release_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
