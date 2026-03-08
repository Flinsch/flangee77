#include "MeshManager.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    /**
     * Creates and acquires the specified vertex shader.
     */
    VertexBuffer::Id MeshManager::_create_vertex_buffer(cl7::u8string_view identifier, const VertexBufferDesc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        ResourcePtr vertex_buffer(_factory->create_vertex_buffer(ctx, desc), _destroy_resource);

        return _try_acquire_and_add_resource<VertexBuffer::Id>(std::move(vertex_buffer), data_provider);
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    IndexBuffer::Id MeshManager::_create_index_buffer(cl7::u8string_view identifier, const IndexBufferDesc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        ResourcePtr index_buffer(_factory->create_index_buffer(ctx, desc), _destroy_resource);

        return _try_acquire_and_add_resource<IndexBuffer::Id>(std::move(index_buffer), data_provider);
    }



} // namespace xl7::graphics::meshes
