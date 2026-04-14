#include "MeshManager.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    /**
     * Creates and acquires the specified vertex buffer.
     */
    VertexBuffer::Id MeshManager::create_vertex_buffer(cl7::u8string_view identifier, const VertexBufferDesc& desc, const VertexBufferWrite* initial_write)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* vertex_buffer = _factory->create_vertex_buffer(ctx, desc);
        ResourcePtr resource_ptr(vertex_buffer, _destroy_resource);

        if (initial_write)
            vertex_buffer->edit().write(*initial_write);
        else
            vertex_buffer->check_is_mutable();

        return _try_acquire_and_add_resource<VertexBuffer::Id>(std::move(resource_ptr));
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    IndexBuffer::Id MeshManager::create_index_buffer(cl7::u8string_view identifier, const IndexBufferDesc& desc, const IndexBufferWrite* initial_write)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* index_buffer = _factory->create_index_buffer(ctx, desc);
        ResourcePtr resource_ptr(index_buffer, _destroy_resource);

        if (initial_write)
            index_buffer->edit().write(*initial_write);
        else
            index_buffer->check_is_mutable();

        return _try_acquire_and_add_resource<IndexBuffer::Id>(std::move(resource_ptr));
    }



} // namespace xl7::graphics::meshes
