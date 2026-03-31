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
        if (write.width == 0 || write.height == 0 || write.depth == 0)
            return true;

        const auto& desc = get_desc();

        if (write.x + write.width > desc.width ||
            write.y + write.height > desc.height ||
            write.z + write.depth > desc.depth)
        {
            LOG_ERROR(u8"The specified texture region exceeds the texture dimensions of the " + get_qualified_identifier() + u8".");
            return false;
        }

        const size_t bytes_per_pixel = PixelLayout::determine_bytes_per_pixel(desc.pixel_format);
        const size_t row_size = write.width * bytes_per_pixel;

        const size_t src_row_pitch = write.row_pitch;
        const size_t src_slice_pitch = write.slice_pitch;

        const size_t dst_row_pitch = desc.width * bytes_per_pixel;
        const size_t dst_slice_pitch = desc.height * dst_row_pitch;
        const size_t dst_layer_pitch = desc.depth * dst_slice_pitch;

        if (src_row_pitch < row_size)
        {
            LOG_ERROR(u8"The specified row pitch is smaller than the required row size for the " + get_qualified_identifier() + u8".");
            return false;
        }

        if (src_slice_pitch < src_row_pitch * write.height)
        {
            LOG_ERROR(u8"The specified slice pitch is smaller than the required slice size for the " + get_qualified_identifier() + u8".");
            return false;
        }

        const size_t expected_size_padded = src_slice_pitch * write.depth;
        const size_t expected_size_cropped =
            src_slice_pitch * (write.depth - 1) +
            src_row_pitch * (write.height - 1) +
            row_size;

        if (write.data.size() != expected_size_padded &&
            write.data.size() != expected_size_cropped)
        {
            LOG_ERROR(u8"The provided data does not match the expected data size for updating the " + get_qualified_identifier() + u8". Expected either padded or tightly packed size.");
            return false;
        }

        for (size_t z = 0; z < write.depth; ++z)
        {
            for (size_t y = 0; y < write.height; ++y)
            {
                const size_t src_offset =
                    z * src_slice_pitch +
                    y * src_row_pitch;

                const size_t dst_offset =
                    write.layer * dst_layer_pitch +
                    (write.z + z) * dst_slice_pitch +
                    (write.y + y) * dst_row_pitch +
                    write.x * bytes_per_pixel;

                const cl7::byte_view row_data = write.data.subspan(src_offset, row_size);

                if (!_write(row_data, dst_offset))
                    return false;
            }
        }

        if (has_custom_dirty_state())
            _update_dirty_state(write.layer, write.x, write.y, write.z, write.width, write.height, write.depth);
        return true;
    }



} // namespace xl7::graphics::textures
