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
    IndexBufferImpl::IndexBufferImpl(const CreateParams<Desc>& params)
        : IndexBuffer( params )
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

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = _size;
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = _stride;

        D3D11_SUBRESOURCE_DATA subresource_data;
        subresource_data.pSysMem = _data.data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        HRESULT hresult = _d3d_device->CreateBuffer(
            &buffer_desc,
            &subresource_data,
            &_d3d_index_buffer );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateBuffer") ) );
            return false;
        }

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
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
