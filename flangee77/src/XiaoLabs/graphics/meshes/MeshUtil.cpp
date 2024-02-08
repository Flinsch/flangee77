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

    /**
     * Calculates the corresponding number of vertices/indices for the specified
     * topology and number of primitives.
     */
    unsigned MeshUtil::calculate_vertex_index_count(Topology topology, unsigned primitive_count)
    {
        switch ( topology )
        {
        case Topology::Unknown:
            return 0;
        case Topology::PointList:
            return primitive_count;
        case Topology::LineList:
            return primitive_count * 2;
        case Topology::LineStrip:
            return primitive_count + 1;
        case Topology::TriangleList:
            return primitive_count * 3;
        case Topology::TriangleStrip:
            return primitive_count + 2;
        default:
            assert( false );
        }

        return 0;
    }

    /**
     * Calculates the corresponding number of primitives for the specified topology
     * and number of vertices/indices.
     */
    unsigned MeshUtil::calculate_primitive_count(Topology topology, unsigned vertex_index_count)
    {
        switch ( topology )
        {
        case Topology::Unknown:
            return 0;
        case Topology::PointList:
            return vertex_index_count;
        case Topology::LineList:
            return vertex_index_count / 2;
        case Topology::LineStrip:
            if ( vertex_index_count < 2 )
                return 0;
            return vertex_index_count - 1;
        case Topology::TriangleList:
            return vertex_index_count / 3;
        case Topology::TriangleStrip:
            if ( vertex_index_count < 3 )
                return 0;
            return vertex_index_count - 2;
        default:
            assert( false );
        }

        return 0;
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
