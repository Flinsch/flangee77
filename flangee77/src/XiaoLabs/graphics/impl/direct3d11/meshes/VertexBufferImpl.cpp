#include "VertexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexBufferImpl::VertexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : VertexBuffer( manager, identifier, desc )
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
    bool VertexBufferImpl::_request_impl()
    {
        return _create_vertex_buffer();
    }

    /**
     * Releases the resource.
     */
    bool VertexBufferImpl::_release_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    bool VertexBufferImpl::_resign_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    bool VertexBufferImpl::_restore_impl()
    {
        if ( !_create_vertex_buffer() )
            return false;

        // 

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the Direct3D 11 index buffer interface.
     */
    bool VertexBufferImpl::_create_vertex_buffer()
    {
        assert( _d3d_device );
        assert( !_d3d_vertex_buffer );

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = this->size;
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = this->desc.vertex_stride;

        HRESULT hresult = _d3d_device->CreateBuffer(
            &buffer_desc,
            nullptr,
            &_d3d_vertex_buffer );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateBuffer") ) );
            return false;
        }

        return true;
    }



} // namespace meshes
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
