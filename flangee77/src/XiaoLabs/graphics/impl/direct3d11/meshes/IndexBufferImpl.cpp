#include "IndexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../RenderingContextImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include "../../shared/meshes/MeshBufferDiscardPolicy.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d11::meshes {



    static DXGI_FORMAT _dxgi_format_from(graphics::meshes::IndexType index_type)
    {
        switch (index_type)
        {
        case graphics::meshes::IndexType::UINT16:
            return DXGI_FORMAT_R16_UINT;
        case graphics::meshes::IndexType::UINT32:
            return DXGI_FORMAT_R32_UINT;
        default:
            assert(false);
        }

        return DXGI_FORMAT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    IndexBufferImpl::IndexBufferImpl(const CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc)
        : IndexBuffer(ctx, desc)
        , _dxgi_format(_dxgi_format_from(desc.index_type))
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

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = get_data_size();
        buffer_desc.Usage = mappings::_d3d_usage_from(get_desc().usage);
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.CPUAccessFlags = get_desc().usage >= graphics::meshes::MeshBufferUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = get_element_stride();

        D3D11_SUBRESOURCE_DATA subresource_data;
        subresource_data.pSysMem = get_data().data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        HRESULT hresult = d3d_device->CreateBuffer(
            &buffer_desc,
            get_dirty_state().is_dirty() ? &subresource_data : nullptr,
            &_d3d_index_buffer);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11Device::CreateBuffer"));
            return false;
        }

        return true;
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

        auto* d3d_device_context = GraphicsSystem::instance().get_rendering_device()->get_primary_context_impl<RenderingContextImpl>()->get_raw_d3d_device_context();
        assert(d3d_device_context);

        const auto& dirty_state = get_dirty_state();

        const auto update = discard_policy.recommend(
            dirty_state.is_all_dirty(),
            dirty_state.first_element() * get_element_stride(),
            dirty_state.element_count() * get_element_stride(),
            get_data_size(),
            get_desc().usage);

        if (get_desc().usage >= graphics::meshes::MeshBufferUsage::Dynamic)
        {
            D3D11_MAP map_type;
            if (update.discard)
                map_type = D3D11_MAP_WRITE_DISCARD;
            else if (get_desc().usage == graphics::meshes::MeshBufferUsage::Transient)
                map_type = D3D11_MAP_WRITE_NO_OVERWRITE;
            else
                map_type = D3D11_MAP_WRITE;

            D3D11_MAPPED_SUBRESOURCE mapped_subresource;
            HRESULT hresult = d3d_device_context->Map(_d3d_index_buffer.Get(), 0, map_type, 0, &mapped_subresource);

            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11DeviceContext::Map"));
                LOG_ERROR(u8"The " + get_qualified_identifier() + u8" could not be mapped for writing.");
                return false;
            }

            std::memcpy(mapped_subresource.pData, get_data().data() + update.offset, update.size);

            d3d_device_context->Unmap(_d3d_index_buffer.Get(), 0);
        }
        else // => _desc.usage == graphics::meshes::MeshBufferUsage::Default
        {
            unsigned copy_flags = 0;
            if (update.discard)
                copy_flags |= D3D11_COPY_DISCARD;
            else if (get_desc().usage == graphics::meshes::MeshBufferUsage::Transient)
                copy_flags |= D3D11_COPY_NO_OVERWRITE;

            D3D11_BOX box;
            box.left = update.offset;
            box.top = 0;
            box.front = 0;
            box.right = update.offset + update.size;
            box.bottom = 1;
            box.back = 1;

            d3d_device_context->UpdateSubresource1(_d3d_index_buffer.Get(), 0, &box, get_data().data() + update.offset, 0, 0, copy_flags);
        }

        return true;
    }



} // namespace xl7::graphics::impl::direct3d11::meshes
