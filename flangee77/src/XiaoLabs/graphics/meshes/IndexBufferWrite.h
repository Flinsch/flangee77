#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFERWRITE_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFERWRITE_H

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::meshes {



struct IndexBufferWrite
{
    /** The provided source index data to write. */
    cl7::byte_view data;

    /** The first index to update, regardless of the topology. */
    unsigned first_index = 0;
    /** The number of indices to update, regardless of the topology. */
    unsigned index_count;



    template <class TIndex>
        requires(sizeof(TIndex) == 2 || sizeof(TIndex) == 4)
    static IndexBufferWrite from_indices(std::span<const TIndex> data, unsigned first_index = 0)
    {
        return {
            .data = cl7::byte_view(reinterpret_cast<const std::byte*>(data.data()), data.size_bytes()),
            .first_index = first_index,
            .index_count = static_cast<unsigned>(data.size()),
        };
    }
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFERWRITE_H
