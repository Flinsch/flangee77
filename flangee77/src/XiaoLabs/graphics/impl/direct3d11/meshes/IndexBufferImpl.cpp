#include "IndexBufferImpl.h"

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
    IndexBufferImpl::IndexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : IndexBuffer( manager, identifier, desc )
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
     * Creates the Direct3D 11 index buffer interface.
     */
    bool IndexBufferImpl::_create_index_buffer()
    {
        assert( _d3d_device );
        assert( !_d3d_index_buffer );

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = this->size;
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = this->stride;

        HRESULT hresult = _d3d_device->CreateBuffer(
            &buffer_desc,
            nullptr,
            &_d3d_index_buffer );

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
