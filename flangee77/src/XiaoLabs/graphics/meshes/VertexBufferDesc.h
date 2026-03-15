#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFERDESC_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFERDESC_H
#include "./MeshBufferDesc.h"

#include "./VertexLayout.h"



namespace xl7::graphics::meshes {



struct VertexBufferDesc
{
    /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
    MeshBufferUsage usage;

    /** The topology of the primitives that the buffer represents. */
    Topology topology;
    /** The number of elements within the buffer, regardless of the topology (i.e., vertices, not primitives). */
    unsigned count;

    /** The size of each vertex, in bytes. */
    unsigned stride;
    /** The layout specification of the vertices. */
    VertexLayout vertex_layout;

    /** The number of instances to draw using the same per-instance data before advancing in the buffer by one element, or simply 0 if the buffer contains "regular" per-vertex data. */
    unsigned instance_data_step_rate;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFERDESC_H
