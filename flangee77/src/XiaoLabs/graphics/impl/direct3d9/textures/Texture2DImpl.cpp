#include "Texture2DImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include "../../shared/textures//TextureDiscardPolicy.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9::textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Texture2DImpl::Texture2DImpl(const CreateContext& ctx, const graphics::textures::Texture2DDesc& desc)
        : Texture2D(ctx, desc)
        , _d3d_format(mappings::_d3d_format_from(desc.pixel_format, get_channel_order()))
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

        HRESULT hresult = d3d_device->CreateTexture(
            get_width(),
            get_height(),
            get_desc().mip_levels,
            mappings::_d3d_usage_from(get_desc().usage, get_desc().mip_levels),
            _d3d_format,
            mappings::_d3d_pool_from(get_desc().usage),
            &_d3d_texture,
            nullptr);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::CreateTexture"));
            return false;
        }

        auto pair = mappings::_map_d3d_format(_d3d_format, get_desc().preferred_channel_order);
        assert(pair.first == get_desc().pixel_format);
        assert(pair.second == get_channel_order());

        return _flush_data();
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool Texture2DImpl::_dispose_impl()
    {
        _d3d_texture.Reset();

        return true;
    }

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool Texture2DImpl::_flush_data_impl()
    {
        static constexpr shared::textures::TextureDiscardPolicy<graphics::textures::TextureExtent2D, graphics::textures::TextureRect> discard_policy;

        const auto& dirty_state = get_dirty_state();

        const auto update = discard_policy.recommend(
            dirty_state.is_all_dirty(),
            dirty_state.region(),
            get_desc().extent,
            get_bytes_per_pixel(),
            get_desc().usage);

        DWORD flags = 0;
        if (update.discard)
            flags |= D3DLOCK_DISCARD;

        constexpr unsigned MAX_LEVELS = 16; // Just some value big enough.
        struct UpdateDesc
        {
            const std::byte* data;
            RECT rect;
            unsigned row_pitch;
        };
        UpdateDesc update_desc[MAX_LEVELS];
        update_desc[0].data = get_data().data() + static_cast<ptrdiff_t>((update.region.y * get_width() + update.region.x) * get_bytes_per_pixel());
        update_desc[0].rect.left = static_cast<LONG>(update.region.x);
        update_desc[0].rect.top = static_cast<LONG>(update.region.y);
        update_desc[0].rect.right = static_cast<LONG>(update.region.x) + static_cast<LONG>(update.region.width);
        update_desc[0].rect.bottom = static_cast<LONG>(update.region.y) + static_cast<LONG>(update.region.height);
        update_desc[0].row_pitch = get_width() * get_bytes_per_pixel();
        unsigned mip_level = 1;

        /*std::vector<images::Image> mipmaps;
        if (get_desc().mip_levels != 1)
        {
            mipmaps = create_mipmaps();
            for (const auto& mipmap : mipmaps)
            {
                if (get_desc().mip_levels != 0 && mip_level >= get_desc().mip_levels)
                    break;
                assert(mip_level < MAX_LEVELS);
                update_desc[mip_level].data = mipmap.get_data().data();
                update_desc[mip_level].rect.left = 0;
                update_desc[mip_level].rect.top = 0;
                update_desc[mip_level].rect.right = static_cast<LONG>(mipmap.get_width());
                update_desc[mip_level].rect.bottom = static_cast<LONG>(mipmap.get_height());
                ++mip_level;
            } // for each mip level
        } // generate mipmaps?*/

        for (unsigned i = 0; i < mip_level; ++i)
        {
            D3DLOCKED_RECT d3d_locked_rect;
            HRESULT hresult = _d3d_texture->LockRect(
                i,
                &d3d_locked_rect,
                &update_desc[i].rect,
                i > 0 ? D3DLOCK_DISCARD : flags);

            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DTexture::LockRect"));
                return false;
            }

            auto* dst = static_cast<std::byte*>(d3d_locked_rect.pBits);
            const std::byte* src = update_desc[i].data;
            auto width = static_cast<unsigned>(update_desc[i].rect.right - update_desc[i].rect.left);
            auto height = static_cast<unsigned>(update_desc[i].rect.bottom - update_desc[i].rect.top);
            unsigned row_pitch = update_desc[i].row_pitch;
            unsigned row_bytes = width * get_bytes_per_pixel();
            for (unsigned y = 0; y < height; ++y)
            {
                std::memcpy(dst, src, row_bytes);
                dst += d3d_locked_rect.Pitch;
                src += row_pitch;
            }

            hresult = _d3d_texture->UnlockRect(i);
            assert(SUCCEEDED(hresult));
        }

        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::textures
