#include "MeshBufferUpdater.h"



namespace xl7::graphics::meshes {



    /**
     * Updates a contiguous range of elements in the mesh buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the element stride, ensuring that only whole elements are written.
     *
     * On success, the corresponding element range is marked as "dirty".
     */
    bool MeshBufferUpdater::write(const MeshBufferWrite& write)
    {
        const size_t element_stride = get_desc().element_stride;
        const size_t offset = write.first_element * element_stride;
        const size_t expected_size = write.element_count * element_stride;

        if (!_check_exact_size(expected_size, write.data.size()))
            return false;

        if (!_check_stride_alignment(element_stride, write.data.size(), offset))
            return false;

        if (!_write(write.data, offset))
            return false;

        _update_dirty_state(write.first_element, write.element_count);
        return true;
    }



} // namespace xl7::graphics::meshes
