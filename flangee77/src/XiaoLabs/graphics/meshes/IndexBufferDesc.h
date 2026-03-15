#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
#include "./MeshBufferDesc.h"

#include "./IndexType.h"



namespace xl7::graphics::meshes {



struct IndexBufferDesc
{
    /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
    MeshBufferUsage usage;

    /** The topology of the primitives that the buffer represents. */
    Topology topology;
    /** The number of elements within the buffer, regardless of the topology (i.e., indices, not primitives). */
    unsigned count;

    /** The type/format (16 or 32 bits) of the index buffer. */
    IndexType index_type;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
