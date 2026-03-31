#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFERWRITE_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFERWRITE_H

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::meshes {



struct VertexBufferWrite
{
    /** The provided source vertex data to write. */
    cl7::byte_view data;

    /** The first vertex to update, regardless of the topology. */
    unsigned first_vertex = 0;
    /** The number of vertices to update, regardless of the topology. */
    unsigned vertex_count;



    template <class TVertex>
        requires(sizeof(TVertex) >= 1)
    static VertexBufferWrite from_vertices(std::span<const TVertex> data, unsigned first_vertex = 0)
    {
        return {
            .data = cl7::byte_view(reinterpret_cast<const std::byte*>(data.data()), data.size_bytes()),
            .first_vertex = first_vertex,
            .vertex_count = static_cast<unsigned>(data.size()),
        };
    }
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFERWRITE_H
