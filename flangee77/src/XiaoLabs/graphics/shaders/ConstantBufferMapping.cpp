#include "ConstantBufferMapping.h"

#include <algorithm>



namespace xl7 {
namespace graphics {
namespace shaders {



    /**
     * Attempts to merge consecutive constant mappings to minimize the number of
     * entries. Ideally, this results in a single mapping entry when there is a
     * continuous 1-to-1 mapping between (sections of) source and shader buffer.
     * The padded sizes of the constant mappings must be set correctly for this
     * operation to be effectively carried out as intended.
     */
    void ConstantBufferMapping::try_merge_constant_mappings()
    {
        std::sort( constant_mappings.begin(), constant_mappings.end(), [](const auto& a, const auto& b) {
            if ( a.slot_index < b.slot_index ) return true;
            if ( a.slot_index > b.slot_index ) return false;
            if ( a.constant_type < b.constant_type ) return true;
            if ( a.constant_type > b.constant_type ) return false;
            // Sorting by either source or shader offset should yield the same
            // result. When constant mappings are consecutive, sorting by one offset
            // automatically sorts the other. Otherwise it wouldn't be a continuous
            // 1-to-1 mapping between (sections of) source and shader buffer anyhow.
            // Our choice falls on shader offset, matching the (shader) index above.
            return a.shader_offset < b.shader_offset;
        } );

        auto _try_flush_merge = [](std::vector<ConstantMapping>& constant_mappings, size_t i, size_t count) {
            assert( i + count <= constant_mappings.size() );
            if ( count <= 1 )
                return;
            auto& base = constant_mappings[ i ];
            const auto& last = constant_mappings[ i + count - 1 ];
            assert( base.source_offset < last.source_offset );
            assert( base.shader_offset < last.shader_offset );
            assert( last.source_offset - base.source_offset == last.shader_offset - base.shader_offset );
            base.size = last.size + last.source_offset - base.source_offset;
            base.padded_size = last.padded_size + last.source_offset - base.source_offset;
            constant_mappings.erase( constant_mappings.begin() + i + 1, constant_mappings.begin() + i + count );
        };

        size_t base_i = 0;
        size_t curr_i = 0;
        while ( ++curr_i < constant_mappings.size() )
        {
            const auto& base = constant_mappings[ base_i ];
            const auto& prev = constant_mappings[ curr_i - 1 ];
            const auto& curr = constant_mappings[ curr_i ];

            assert( prev.slot_index == base.slot_index );
            bool continue_merge = curr.slot_index == prev.slot_index;
            continue_merge &= curr.constant_type == prev.constant_type;

            assert( !continue_merge || curr.shader_offset > prev.shader_offset );
            continue_merge &= curr.source_offset == prev.source_offset + prev.padded_size;
            continue_merge &= curr.shader_offset == prev.shader_offset + prev.padded_size;
            assert( !continue_merge || curr.source_offset > prev.source_offset );
            assert( !continue_merge || curr.source_offset - base.source_offset == curr.shader_offset - base.shader_offset );

            if ( continue_merge )
                continue;

            _try_flush_merge( constant_mappings, base_i, curr_i - base_i );

            curr_i = ++base_i;
        }

        _try_flush_merge( constant_mappings, base_i, curr_i - base_i );
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
