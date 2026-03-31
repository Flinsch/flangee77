#include "Texture2DArrayUpdater.h"

#include "./Texture.h"



namespace xl7::graphics::textures {



    /**
     * Updates a rectangular region of a single layer of a 2D texture array, and
     * returns true if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row pitch requirements, ensuring that only
     * whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool Texture2DArrayUpdater::write(const Texture2DArrayWrite& write)
    {
        const auto& desc = get_desc();

        TextureUpdater(dynamic_cast<const Texture*>(get_resource()), {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .layer_count = desc.layer_count,
        }, _access_data(), nullptr).write({
            .data = write.data,
            .x = write.x,
            .y = write.y,
            .z = 0,
            .width = write.width,
            .height = write.height,
            .depth = 1,
            .layer = write.layer,
            .row_pitch = write.row_pitch,
            .slice_pitch = write.height * write.row_pitch,
        });

        _update_dirty_state(write.layer, write.x, write.y, write.width, write.height);
        return true;
    }



} // namespace xl7::graphics::textures
