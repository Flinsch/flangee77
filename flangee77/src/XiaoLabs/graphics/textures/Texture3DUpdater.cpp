#include "Texture3DUpdater.h"

#include "./Texture.h"



namespace xl7::graphics::textures {



    /**
     * Updates a cubic region of a 3D texture, and returns true if the update
     * succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row/slice pitch requirements, ensuring that
     * only whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool Texture3DUpdater::write(const Texture3DWrite& write)
    {
        const auto& region = write.region;
        const auto& desc = get_desc();

        TextureUpdater(dynamic_cast<const Texture*>(get_resource()), {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .extent = desc.extent.generalize(),
            .layer_count = 1,
        }, _access_data(), nullptr).write({
            .data = write.data,
            .region = region.generalize(),
            .layer = 0,
            .row_pitch = write.row_pitch,
            .slice_pitch = write.slice_pitch,
        });

        _update_dirty_state(region);
        return true;
    }



} // namespace xl7::graphics::textures
