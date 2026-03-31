#include "CubemapUpdater.h"

#include "./Texture.h"



namespace xl7::graphics::textures {



    /**
     * Updates a rectangular region of a cubemap face, and returns true if the
     * update succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row pitch requirements, ensuring that only
     * whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool CubemapUpdater::write(const CubemapWrite& write)
    {
        const auto& desc = get_desc();

        const auto layer = static_cast<unsigned>(write.face);
        assert(layer < 6);

        TextureUpdater(dynamic_cast<const Texture*>(get_resource()), {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .layer_count = 6,
        }, _access_data(), nullptr).write({
            .data = write.data,
            .x = write.x,
            .y = write.y,
            .z = 0,
            .width = write.width,
            .height = write.height,
            .depth = 1,
            .layer = layer,
            .row_pitch = write.row_pitch,
            .slice_pitch = write.height * write.row_pitch,
        });

        _update_dirty_state(layer, write.x, write.y, write.width, write.height);
        return true;
    }



} // namespace xl7::graphics::textures
