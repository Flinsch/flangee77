#include "Texture2DImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../RenderingContextImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include "../../shared/textures/TextureDiscardPolicy.h"

#include "../../../images/ImageResizer.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d11::textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Texture2DImpl::Texture2DImpl(const CreateContext& ctx, const graphics::textures::Texture2DDesc& desc)
        : Texture2D(ctx, desc)
        , _dxgi_format(mappings::_dxgi_format_from(desc.pixel_format, get_channel_order()))
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool Texture2DImpl::_acquire_impl()
    {
        auto* d3d_device = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_raw_d3d_device();
        assert(d3d_device);

        unsigned mip_levels = get_desc().mip_levels;
        if (get_desc().usage >= graphics::textures::TextureUsage::Dynamic && mip_levels != 1)
        {
            mip_levels = 1;
            LOG_WARNING(u8"A dynamic texture cannot have mipmaps, so the number of mipmap levels is set to 1.");
        }

        D3D11_TEXTURE2D_DESC texture_desc;
        texture_desc.Width = get_width();
        texture_desc.Height = get_height();
        texture_desc.MipLevels = mip_levels;
        texture_desc.ArraySize = 1;
        texture_desc.Format = _dxgi_format;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = mappings::_d3d_usage_from(get_desc().usage);
        texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags = get_desc().usage >= graphics::textures::TextureUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        texture_desc.MiscFlags = 0;

        constexpr unsigned MAX_LEVELS = 16; // Just some value big enough.
        D3D11_SUBRESOURCE_DATA subresource_data[MAX_LEVELS];
        subresource_data[0].pSysMem = get_data().data();
        subresource_data[0].SysMemPitch = get_row_pitch();
        subresource_data[0].SysMemSlicePitch = 0;

        std::vector<images::Image> mipmaps;
        if (get_dirty_state().is_dirty() && mip_levels != 1)
        {
            mipmaps = create_mipmaps();
            unsigned mip_level = 1;
            for (const auto& mipmap : mipmaps)
            {
                if (mip_levels != 0 && mip_level >= mip_levels)
                    break;
                assert(mip_level < MAX_LEVELS);
                subresource_data[mip_level].pSysMem = mipmap.get_data().data();
                subresource_data[mip_level].SysMemPitch = get_bytes_per_pixel() * mipmap.get_width();
                subresource_data[mip_level].SysMemSlicePitch = 0;
                ++mip_level;
            } // for each mip level
        } // generate mipmaps?

        HRESULT hresult = d3d_device->CreateTexture2D(
            &texture_desc,
            get_dirty_state().is_dirty() ? subresource_data : nullptr,
            &_d3d_texture);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11Device::CreateTexture2D"));
            return false;
        }

        hresult = d3d_device->CreateShaderResourceView(
            _d3d_texture.Get(),
            nullptr,
            &_d3d_shader_resource_view);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11Device::CreateShaderResourceView"));
            return false;
        }

        auto pair = mappings::_map_dxgi_format(_dxgi_format, get_desc().preferred_channel_order);
        assert(pair.first == get_desc().pixel_format);
        assert(pair.second == get_channel_order());

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool Texture2DImpl::_dispose_impl()
    {
        _d3d_shader_resource_view.Reset();
        _d3d_texture.Reset();

        return true;
    }

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool Texture2DImpl::_flush_data_impl()
    {
        static constexpr shared::textures::TextureDiscardPolicy discard_policy;

        auto* d3d_device_context = GraphicsSystem::instance().get_rendering_device()->get_primary_context_impl<RenderingContextImpl>()->get_raw_d3d_device_context();
        assert(d3d_device_context);

        const auto& dirty_state = get_dirty_state();

        const auto update = discard_policy.recommend(
            dirty_state.is_all_dirty(),
            dirty_state.region().generalize(),
            get_desc().extent.generalize(),
            get_bytes_per_pixel(),
            get_desc().usage);

        if (get_desc().usage >= graphics::textures::TextureUsage::Dynamic)
        {
            D3D11_MAP map_type;
            if (update.discard)
                map_type = D3D11_MAP_WRITE_DISCARD;
            else
                map_type = D3D11_MAP_WRITE;

            D3D11_MAPPED_SUBRESOURCE mapped_subresource;
            HRESULT hresult = d3d_device_context->Map(_d3d_texture.Get(), 0, map_type, 0, &mapped_subresource);

            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11DeviceContext::Map"));
                LOG_ERROR(u8"The " + get_qualified_identifier() + u8" could not be mapped for writing.");
                return false;
            }

            auto* dst = static_cast<std::byte*>(mapped_subresource.pData);
            const std::byte* src = get_data().data() + static_cast<ptrdiff_t>((update.region.y * get_width() + update.region.x) * get_bytes_per_pixel());
            unsigned row_pitch = get_row_pitch();
            unsigned row_bytes = update.region.width * get_bytes_per_pixel();
            for (unsigned y = 0; y < update.region.height; ++y)
            {
                std::memcpy(dst, src, row_bytes);
                dst += mapped_subresource.RowPitch;
                src += row_pitch;
            }

            d3d_device_context->Unmap(_d3d_texture.Get(), 0);
        }
        else // => _desc.usage == graphics::textures::TextureUsage::Default
        {
            unsigned copy_flags = 0;
            if (update.discard)
                copy_flags |= D3D11_COPY_DISCARD;

            D3D11_BOX box;
            box.left = update.region.x;
            box.top = update.region.y;
            box.front = 0;
            box.right = update.region.width;
            box.bottom = update.region.height;
            box.back = 1;

            d3d_device_context->UpdateSubresource1(_d3d_texture.Get(), 0, &box, get_data().data(), get_row_pitch(), 0, copy_flags);

            if (get_desc().mip_levels != 1)
            {
                std::vector<images::Image> mipmaps = create_mipmaps();
                unsigned mip_level = 1;
                for (const auto& mipmap : mipmaps)
                {
                    if (get_desc().mip_levels != 0 && mip_level >= get_desc().mip_levels)
                        break;

                    box.left = 0;
                    box.top = 0;
                    box.front = 0;
                    box.right = mipmap.get_width();
                    box.bottom = mipmap.get_height();
                    box.back = 1;

                    d3d_device_context->UpdateSubresource1(_d3d_texture.Get(), mip_level, &box, mipmap.get_data().data(), mipmap.get_width() * get_bytes_per_pixel(), 0, D3D11_COPY_DISCARD);

                    ++mip_level;
                } // for each mip level
            } // generate mipmaps?
        }

        return true;
    }



} // namespace xl7::graphics::impl::direct3d11::textures
