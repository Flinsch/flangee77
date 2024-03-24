#pragma once
#ifndef XL7_GRAPHICS_IMPL_SHARED_MESHES_COMPOSEDVERTEXLAYOUT_H
#define XL7_GRAPHICS_IMPL_SHARED_MESHES_COMPOSEDVERTEXLAYOUT_H

#include "./VertexBufferBinding.h"

#include "../../../pipeline/InputAssemblerStage.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace meshes {



struct ComposedVertexLayout
{

    xl7::graphics::meshes::VertexLayout vertex_layouts[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
    unsigned instance_data_step_rates[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
    unsigned stream_count;


    ComposedVertexLayout();
    ComposedVertexLayout(const VertexBufferBinding& vertex_buffer_binding);

    bool operator == (const ComposedVertexLayout& rhs) const;

    size_t hash() const;

}; // struct ComposedVertexLayout



} // namespace meshes
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7



template <>
struct std::hash<xl7::graphics::impl::shared::meshes::ComposedVertexLayout>
{
    size_t operator () (const xl7::graphics::impl::shared::meshes::ComposedVertexLayout& composed_vertex_layout) const
    {
        return composed_vertex_layout.hash();
    }
};



#endif // XL7_GRAPHICS_IMPL_SHARED_MESHES_COMPOSEDVERTEXLAYOUT_H
