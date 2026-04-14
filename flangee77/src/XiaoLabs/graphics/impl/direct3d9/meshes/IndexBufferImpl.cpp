#include "IndexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include "../../shared/meshes/MeshBufferDiscardPolicy.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9::meshes {



    static D3DFORMAT _d3d_format_from(graphics::meshes::IndexType index_type)
    {
        switch (index_type)
        {
        case graphics::meshes::IndexType::UINT16:
            return D3DFMT_INDEX16;
        case graphics::meshes::IndexType::UINT32:
            return D3DFMT_INDEX32;
        default:
            assert(false);
        }

        return D3DFMT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    IndexBufferImpl::IndexBufferImpl(const CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc)
        : IndexBuffer(ctx, desc)
        , _d3d_format(_d3d_format_from(desc.index_type))
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool IndexBufferImpl::_acquire_impl()
    {
        auto* d3d_device = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_raw_d3d_device();
        assert(d3d_device);

        HRESULT hresult = d3d_device->CreateIndexBuffer(
            get_data_size(),
            mappings::_d3d_usage_from(get_desc().usage),
            _d3d_format,
            mappings::_d3d_pool_from(get_desc().usage),
            &_d3d_index_buffer,
            nullptr);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::CreateIndexBuffer"));
            return false;
        }

        return _flush_data();
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool IndexBufferImpl::_dispose_impl()
    {
        _d3d_index_buffer.Reset();

        return true;
    }

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool IndexBufferImpl::_flush_data_impl()
    {
        static constexpr shared::meshes::MeshBufferDiscardPolicy discard_policy;

        const auto& dirty_state = get_dirty_state();

        const auto update = discard_policy.recommend(
            dirty_state.is_all_dirty(),
            dirty_state.first_element() * get_element_stride(),
            dirty_state.element_count() * get_element_stride(),
            get_data_size(),
            get_desc().usage);

        DWORD flags = 0;
        if (update.discard)
            flags |= D3DLOCK_DISCARD;
        else if (get_desc().usage == graphics::meshes::MeshBufferUsage::Transient)
            flags |= D3DLOCK_NOOVERWRITE;

        void* dst;
        HRESULT hresult = _d3d_index_buffer->Lock(
            update.offset,
            update.size,
            &dst,
            flags);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DIndexBuffer9::Lock"));
            return false;
        }

        std::memcpy(dst, get_data().data() + update.offset, update.size);

        hresult = _d3d_index_buffer->Unlock();
        assert(SUCCEEDED(hresult));

        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::meshes
