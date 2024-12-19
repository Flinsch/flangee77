#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHUTIL_H
#define XL7_GRAPHICS_MESHES_MESHUTIL_H

#include "./VertexLayout.h"
#include "./IndexType.h"
#include "./Topology.h"



namespace xl7::graphics::meshes {



struct MeshUtil
{

    /**
     * Calculates the minimum expected size of a vertex of the specified vertex
     * layout, in bytes.
     */
    static unsigned calculate_default_vertex_stride(const VertexLayout& vertex_layout);

    /**
     * Returns the size of an index of the specified index type, in bytes.
     */
    static unsigned get_index_stride(IndexType index_type);

    /**
     * Calculates the corresponding number of vertices/indices for the specified
     * topology and number of primitives.
     */
    static unsigned calculate_vertex_index_count(Topology topology, unsigned primitive_count);

    /**
     * Calculates the corresponding number of primitives for the specified topology
     * and number of vertices/indices.
     */
    static unsigned calculate_primitive_count(Topology topology, unsigned vertex_index_count);

}; // struct MeshUtil



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHUTIL_H
