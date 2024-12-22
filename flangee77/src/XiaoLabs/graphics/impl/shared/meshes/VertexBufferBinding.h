#pragma once
#ifndef XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H
#define XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H

#include "../../../pipeline/InputAssemblerStage.h"



namespace xl7::graphics::impl::shared::meshes {



struct VertexBufferBinding
{

    resources::ResourceID vertex_buffer_ids[pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS];
    unsigned stream_count;


    VertexBufferBinding();

    bool operator==(const VertexBufferBinding& rhs) const noexcept;

    size_t hash() const noexcept;

}; // struct VertexBufferBinding



} // namespace xl7::graphics::impl::shared::meshes



template <>
struct std::hash<xl7::graphics::impl::shared::meshes::VertexBufferBinding>
{
    size_t operator()(const xl7::graphics::impl::shared::meshes::VertexBufferBinding& vertex_buffer_binding) const noexcept
    {
        return vertex_buffer_binding.hash();
    }
};



#endif // XL7_GRAPHICS_IMPL_SHARED_MESHES_VERTEXBUFFERBINDING_H
