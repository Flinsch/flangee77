#ifndef XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H
#define XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H

#include "./MeshBufferUsage.h"
#include "./Topology.h"



namespace xl7::graphics::meshes {



struct MeshBufferDesc
{
    /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
    MeshBufferUsage usage;

    /** The topology of the primitives that the buffer represents. */
    Topology topology;
    /** The number of elements within the buffer, regardless of the topology (i.e., vertices or indices, not primitives). */
    unsigned count;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFERDESC_H
