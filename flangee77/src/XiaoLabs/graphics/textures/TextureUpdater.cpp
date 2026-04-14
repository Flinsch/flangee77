#include "TextureUpdater.h"

#include "../PixelLayout.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    /**
     * Updates a region of a texture, and returns true if the update succeeded,
     * false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row/slice pitch requirements, ensuring that
     * only whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool TextureUpdater::write(const TextureWrite& write)
    {
        const auto& region = write.region;

        if (region.to_extent().volume() == 0)
            return true;

        const auto& desc = get_desc();

        if (region.x + region.width > desc.extent.width ||
            region.y + region.height > desc.extent.height ||
            region.z + region.depth > desc.extent.depth)
        {
            LOG_ERROR(u8"The specified texture region exceeds the texture dimensions of the " + get_qualified_identifier() + u8".");
            return false;
        }

        const size_t bytes_per_pixel = PixelLayout::determine_bytes_per_pixel(desc.pixel_format);
        const size_t row_size = region.width * bytes_per_pixel;

        const size_t src_row_pitch = write.row_pitch;
        const size_t src_slice_pitch = write.slice_pitch;

        const size_t dst_row_pitch = desc.extent.width * bytes_per_pixel;
        const size_t dst_slice_pitch = desc.extent.height * dst_row_pitch;
        const size_t dst_layer_pitch = desc.extent.depth * dst_slice_pitch;

        if (src_row_pitch < row_size)
        {
            LOG_ERROR(u8"The specified row pitch is smaller than the required row size for the " + get_qualified_identifier() + u8".");
            return false;
        }

        if (src_slice_pitch < src_row_pitch * region.height)
        {
            LOG_ERROR(u8"The specified slice pitch is smaller than the required slice size for the " + get_qualified_identifier() + u8".");
            return false;
        }

        const size_t expected_size_padded = src_slice_pitch * region.depth;
        const size_t expected_size_cropped =
            src_slice_pitch * (region.depth - 1) +
            src_row_pitch * (region.height - 1) +
            row_size;

        if (write.data.size() != expected_size_padded &&
            write.data.size() != expected_size_cropped)
        {
            LOG_ERROR(u8"The provided data does not match the expected data size for updating the " + get_qualified_identifier() + u8". Expected either padded or tightly packed size.");
            return false;
        }

        for (size_t z = 0; z < region.depth; ++z)
        {
            for (size_t y = 0; y < region.height; ++y)
            {
                const size_t src_offset =
                    z * src_slice_pitch +
                    y * src_row_pitch;

                const size_t dst_offset =
                    write.layer * dst_layer_pitch +
                    (region.z + z) * dst_slice_pitch +
                    (region.y + y) * dst_row_pitch +
                    region.x * bytes_per_pixel;

                const cl7::byte_view row_data = write.data.subspan(src_offset, row_size);

                if (!_write(row_data, dst_offset))
                    return false;
            }
        }

        if (has_custom_dirty_state())
            _update_dirty_state(write.layer, region.x, region.y, region.z, region.width, region.height, region.depth);
        return true;
    }



} // namespace xl7::graphics::textures
