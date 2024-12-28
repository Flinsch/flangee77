#include "VertexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9::meshes {



    static DWORD _d3d_fvf_from(const xl7::graphics::meshes::VertexLayout& vertex_layout)
    {
        

        return D3DFMT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    VertexBufferImpl::VertexBufferImpl(const CreateParams<Desc>& params)
        : VertexBuffer(params)
        , _d3d_fvf(_d3d_fvf_from(get_desc().vertex_layout))
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
    bool VertexBufferImpl::_acquire_impl(const resources::DataProvider& data_provider)
    {
        auto* d3d_device = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_raw_d3d_device();
        assert(d3d_device);

        assert(get_data().empty() || get_data().size() == static_cast<size_t>(get_size()));

        HRESULT hresult = d3d_device->CreateVertexBuffer(
            get_size(),
            mappings::_d3d_usage_from(get_desc().usage),
            _d3d_fvf,
            mappings::_d3d_pool_from(get_desc().usage),
            &_d3d_vertex_buffer,
            nullptr);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::CreateVertexBuffer"));
            return false;
        }

        if (get_data().empty())
            return true;

        return _update_impl(data_provider, true, true);
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool VertexBufferImpl::_dispose_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }



    // #############################################################################
    // VertexBuffer Implementations
    // #############################################################################

    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool VertexBufferImpl::_update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite)
    {
        DWORD flags = 0;
        if (discard)
            flags |= D3DLOCK_DISCARD;
        else if (no_overwrite)
            flags |= D3DLOCK_NOOVERWRITE;

        void* dst;
        HRESULT hresult = _d3d_vertex_buffer->Lock(
            static_cast<unsigned>(data_provider.get_offset()),
            static_cast<unsigned>(data_provider.get_size()),
            &dst,
            flags);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DVertexBuffer9::Lock"));
            return false;
        }

        ::memcpy(dst, get_data().data() + data_provider.get_offset(), data_provider.get_size());

        hresult = _d3d_vertex_buffer->Unlock();
        assert(SUCCEEDED(hresult));

        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::meshes
