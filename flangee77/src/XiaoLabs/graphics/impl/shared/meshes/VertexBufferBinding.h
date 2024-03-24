#pragma once
#ifndef XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H
#define XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H

#include "../../../pipeline/InputAssemblerStage.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace meshes {



struct VertexBufferBinding
{

    resources::ResourceID vertex_buffer_ids[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
    unsigned stream_count;


    VertexBufferBinding();

    bool operator == (const VertexBufferBinding& rhs) const;

    size_t hash() const;

}; // struct VertexBufferBinding



} // namespace meshes
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7



template <>
struct std::hash<xl7::graphics::impl::shared::meshes::VertexBufferBinding>
{
    size_t operator () (const xl7::graphics::impl::shared::meshes::VertexBufferBinding& vertex_buffer_binding) const
    {
        return vertex_buffer_binding.hash();
    }
};



#endif // XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H
