#ifndef XL7_GRAPHICS_IMPL_SHARED_MESHES_MESHBUFFERDISCARDPOLICY_H
#define XL7_GRAPHICS_IMPL_SHARED_MESHES_MESHBUFFERDISCARDPOLICY_H

#include "../../../meshes/MeshBufferUsage.h"



namespace xl7::graphics::impl::shared::meshes {



/**
 * Decides whether a hardware vertex/index buffer update should discard the
 * entire buffer contents instead of updating only the dirty subrange.
 */
class MeshBufferDiscardPolicy
{
public:
    struct DiscardThresholds
    {
        /** Buffers smaller than this size (in bytes) are always discarded. */
        unsigned small_buffer_threshold = 128 * 1024; // 128 KiB
        /** Discards if the dirty range exceeds this fraction of the total buffer size. */
        float large_dirty_fraction = 0.5f; // 50 %
        /** Discards dynamic buffers if the dirty range exceeds this fraction of the total buffer size. */
        float dynamic_dirty_fraction = 0.2f; // 20 %
    };

    struct UpdateRecommendation
    {
        /** The offset of the update range, in bytes. */
        unsigned offset;
        /** The size of the update range, in bytes. */
        unsigned size;
        /** Whether to discard the entire buffer data. */
        bool discard;
    };



    MeshBufferDiscardPolicy() = default;
    explicit MeshBufferDiscardPolicy(const DiscardThresholds& thresholds) : _thresholds(thresholds) {}



    /**
     * Returns the recommended update range and whether this would mean discarding
     * all buffer data. Offset and sizes are in bytes.
     */
    UpdateRecommendation recommend(bool all_dirty, unsigned dirty_offset, unsigned dirty_size, unsigned total_size, graphics::meshes::MeshBufferUsage usage) const;

    /**
     * Returns true if the buffer data should be fully discarded instead of only
     * updating the specified dirty range. Sizes are in bytes.
     */
    bool should_discard(unsigned dirty_size, unsigned total_size, graphics::meshes::MeshBufferUsage usage) const;



private:
    DiscardThresholds _thresholds{};

}; // class MeshBufferDiscardPolicy



} // namespace xl7::graphics::impl::shared::meshes

#endif // XL7_GRAPHICS_IMPL_SHARED_MESHES_MESHBUFFERDISCARDPOLICY_H
