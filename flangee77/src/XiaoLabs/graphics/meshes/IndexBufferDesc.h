#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
#include "./MeshBufferDesc.h"

#include "./IndexType.h"



namespace xl7::graphics::meshes {



struct IndexBufferDesc
    : MeshBufferDesc
{
    /** The type/format (16 or 32 bits) of the index buffer. */
    IndexType index_type;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFERDESC_H
