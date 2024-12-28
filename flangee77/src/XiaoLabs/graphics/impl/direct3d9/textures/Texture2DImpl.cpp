#include "Texture2DImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9::textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Texture2DImpl::Texture2DImpl(const CreateParams<Desc>& params)
        : Texture2D(params)
        , _d3d_format(mappings::_d3d_format_from(params.desc.pixel_format, get_channel_order()))
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool Texture2DImpl::_dispose_impl()
    {
        _d3d_texture.Reset();

        return true;
    }



    // #############################################################################
    // Texture2D Implementations
    // #############################################################################

    /**
     * Requests/acquires the texture resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture2DImpl::_acquire_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider)
    {
        auto* d3d_device = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_raw_d3d_device();
        assert(d3d_device);

        assert(get_data().empty() || get_data().size() == static_cast<size_t>(get_data_size()));

        HRESULT hresult = d3d_device->CreateTexture(
            get_desc().width,
            get_desc().height,
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

        if (get_data().empty())
            return true;

        return _update_impl(image_data_provider, true, true);
    }

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture2DImpl::_update_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider, bool discard, bool no_overwrite)
    {
        DWORD flags = D3DLOCK_DISCARD;

        constexpr unsigned MAX_LEVELS = 16; // Just some value big enough.
        struct UpdateDesc
        {
            const std::byte* data;
            RECT rect;
        };
        UpdateDesc update_desc[MAX_LEVELS];
        update_desc[0].data = get_data().data();
        update_desc[0].rect.left = 0;
        update_desc[0].rect.top = 0;
        update_desc[0].rect.right = static_cast<LONG>(get_desc().width);
        update_desc[0].rect.bottom = static_cast<LONG>(get_desc().height);
        unsigned mip_level = 1;

        /*std::vector<xl7::graphics::images::Image> mipmaps;
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
            unsigned pitch = width * get_stride();
            for (unsigned y = 0; y < height; ++y)
            {
                ::memcpy(dst, src, pitch);
                dst += d3d_locked_rect.Pitch;
                src += pitch;
            }

            hresult = _d3d_texture->UnlockRect(i);
            assert(SUCCEEDED(hresult));
        }

        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::textures
