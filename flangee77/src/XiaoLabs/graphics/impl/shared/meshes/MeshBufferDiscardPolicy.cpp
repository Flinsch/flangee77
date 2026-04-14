#include "MeshBufferDiscardPolicy.h"



namespace xl7::graphics::impl::shared::meshes {



    /**
     * Returns the recommended update range and whether this would mean discarding
     * all buffer data. Offset and sizes are in bytes.
     */
    MeshBufferDiscardPolicy::UpdateRecommendation MeshBufferDiscardPolicy::recommend(bool all_dirty, unsigned dirty_offset, unsigned dirty_size, unsigned total_size, graphics::meshes::MeshBufferUsage usage) const
    {
        if (all_dirty || should_discard(dirty_size, total_size, usage))
            return {.offset = 0, .size = total_size, .discard = true};

        return {.offset = dirty_offset, .size = dirty_size, .discard = false};
    }

    /**
     * Returns true if the buffer data should be fully discarded instead of only
     * updating the specified dirty range. Sizes are in bytes.
     */
    bool MeshBufferDiscardPolicy::should_discard(unsigned dirty_size, unsigned total_size, graphics::meshes::MeshBufferUsage usage) const
    {
        if (dirty_size >= total_size)
            return true;

        if (total_size < _thresholds.small_buffer_threshold)
            return true;

        const float dirty_fraction = static_cast<float>(dirty_size) / static_cast<float>(total_size);

        if (dirty_fraction >= _thresholds.large_dirty_fraction)
            return true;

        if (usage == graphics::meshes::MeshBufferUsage::Dynamic &&
            dirty_fraction >= _thresholds.dynamic_dirty_fraction)
            return true;

        return false;
    }



} // namespace xl7::graphics::impl::shared::meshes
