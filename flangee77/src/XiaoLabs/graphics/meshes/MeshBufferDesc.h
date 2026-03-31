#ifndef XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H
#define XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H

#include "./MeshBufferUsage.h"
#include "./Topology.h"



namespace xl7::graphics::meshes {



struct MeshBufferDesc
{
    /** Identifies how the buffer is expected to be updated. */
    MeshBufferUsage usage;

    /** The topology of the primitives that the buffer represents. */
    Topology topology;
    /** The number of elements within the buffer, regardless of the topology (not bytes, not primitives). */
    unsigned element_count;

    /** The size of each buffer element, in bytes. */
    unsigned element_stride;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H
