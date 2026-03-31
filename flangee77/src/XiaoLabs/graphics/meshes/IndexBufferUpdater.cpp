#include "IndexBufferUpdater.h"

#include "./MeshBuffer.h"
#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



    /**
     * Updates a contiguous range of indices in the index buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the index format, ensuring that only whole indices are written.
     *
     * On success, the corresponding index range is marked as "dirty".
     */
    bool IndexBufferUpdater::write(const IndexBufferWrite& write)
    {
        const auto& desc = get_desc();

        return MeshBufferUpdater(dynamic_cast<const MeshBuffer*>(get_resource()), {
            .usage = desc.usage,
            .topology = desc.topology,
            .element_count = desc.index_count,
            .element_stride = MeshUtil::get_index_stride(desc.index_type),
        }, _access_data(), &_access_dirty_state()).write({
            .data = write.data,
            .first_element = write.first_index,
            .element_count = write.index_count,
        });
    }



} // namespace xl7::graphics::meshes
