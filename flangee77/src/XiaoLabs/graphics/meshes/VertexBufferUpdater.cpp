#include "VertexBufferUpdater.h"

#include "./MeshBuffer.h"



namespace xl7::graphics::meshes {



    /**
     * Updates a contiguous range of vertices in the vertex buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the vertex stride, ensuring that only whole vertices are written.
     *
     * On success, the corresponding vertex range is marked as "dirty".
     */
    bool VertexBufferUpdater::write(const VertexBufferWrite& write)
    {
        const auto& desc = get_desc();

        return MeshBufferUpdater(dynamic_cast<const MeshBuffer*>(get_resource()), {
            .usage = desc.usage,
            .topology = desc.topology,
            .element_count = desc.vertex_count,
            .element_stride = desc.vertex_stride,
        }, _access_data(), &_access_dirty_state()).write({
            .data = write.data,
            .first_element = write.first_vertex,
            .element_count = write.vertex_count,
        });
    }



} // namespace xl7::graphics::meshes
