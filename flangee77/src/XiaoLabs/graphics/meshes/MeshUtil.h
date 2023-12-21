#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHUTIL_H
#define XL7_GRAPHICS_MESHES_MESHUTIL_H

#include "./VertexLayout.h"
#include "./IndexType.h"
#include "./Topology.h"



namespace xl7 {
namespace graphics {
namespace meshes {



struct MeshUtil
{

    /**
     * Calculates the minimum expected size of a vertex of the specified vertex
     * layout, in bytes.
     */
    static unsigned calculate_default_vertex_stride(const VertexLayout& vertex_layout);

    /**
     * Calculates the size of an index of the specified index type, in bytes.
     */
    static unsigned calculate_index_stride(IndexType index_type);

}; // struct MeshUtil



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_MESHUTIL_H
