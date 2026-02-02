#include "VertexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../RenderingContextImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d11::meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    VertexBufferImpl::VertexBufferImpl(const CreateParams<Desc>& params)
        : VertexBuffer(params)
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

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = get_size();
        buffer_desc.Usage = mappings::_d3d_usage_from(get_desc().usage);
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = get_desc().usage >= graphics::meshes::MeshBufferUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = get_desc().stride;

        D3D11_SUBRESOURCE_DATA subresource_data;
        subresource_data.pSysMem = get_data().data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        HRESULT hresult = d3d_device->CreateBuffer(
            &buffer_desc,
            get_data().empty() ? nullptr : &subresource_data,
            &_d3d_vertex_buffer);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11Device::CreateBuffer"));
            return false;
        }

        return true;
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
        auto* d3d_device_context = GraphicsSystem::instance().get_rendering_device()->get_primary_context_impl<RenderingContextImpl>()->get_raw_d3d_device_context();
        assert(d3d_device_context);

        if (get_desc().usage >= graphics::meshes::MeshBufferUsage::Dynamic)
        {
            D3D11_MAP map_type;
            if (discard)
                map_type = D3D11_MAP_WRITE_DISCARD;
            else if (no_overwrite)
                map_type = D3D11_MAP_WRITE_NO_OVERWRITE;
            else
                map_type = D3D11_MAP_WRITE;

            D3D11_MAPPED_SUBRESOURCE mapped_subresource;
            HRESULT hresult = d3d_device_context->Map(_d3d_vertex_buffer.Get(), 0, map_type, 0, &mapped_subresource);

            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11DeviceContext::Map"));
                LOG_ERROR(u8"The " + get_typed_identifier_string() + u8" could not be mapped for writing.");
                return false;
            }

            std::memcpy(mapped_subresource.pData, get_data().data() + data_provider.get_offset(), data_provider.get_size());

            d3d_device_context->Unmap(_d3d_vertex_buffer.Get(), 0);
        }
        else // => _desc.usage == graphics::meshes::MeshBufferUsage::Default
        {
            unsigned copy_flags = 0;
            if (discard)
                copy_flags |= D3D11_COPY_DISCARD;
            else if (no_overwrite)
                copy_flags |= D3D11_COPY_NO_OVERWRITE;

            D3D11_BOX box;
            box.left = static_cast<unsigned>(data_provider.get_offset());
            box.top = 0;
            box.front = 0;
            box.right = static_cast<unsigned>(data_provider.get_offset() + data_provider.get_size());
            box.bottom = 1;
            box.back = 1;

            d3d_device_context->UpdateSubresource1(_d3d_vertex_buffer.Get(), 0, &box, get_data().data() + data_provider.get_offset(), 0, 0, copy_flags);
        }

        return true;
    }



} // namespace xl7::graphics::impl::direct3d11::meshes
