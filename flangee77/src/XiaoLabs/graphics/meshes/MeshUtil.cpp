#include "MeshUtil.h"



namespace xl7 {
namespace graphics {
namespace meshes {



    /**
     * Calculates the minimum expected size of a vertex of the specified vertex
     * layout, in bytes.
     */
    unsigned MeshUtil::calculate_default_vertex_stride(const VertexLayout& vertex_layout)
    {
        return vertex_layout.calculate_size();
    }

    /**
     * Calculates the size of an index of the specified index type, in bytes.
     */
    unsigned MeshUtil::calculate_index_stride(IndexType index_type)
    {
        switch ( index_type )
        {
        case IndexType::UINT16:
            return 2;
        case IndexType::UINT32:
            return 4;
        default:
            assert( false );
        }

        return 0;
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
