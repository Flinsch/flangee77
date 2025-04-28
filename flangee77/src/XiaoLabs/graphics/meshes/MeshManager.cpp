#include "MeshManager.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    /**
     * Creates and acquires the specified vertex shader.
     */
    VertexBuffer::ID MeshManager::_create_vertex_buffer(cl7::u8string_view identifier, const VertexBuffer::Desc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateParams<VertexBuffer::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr vertex_buffer(_factory->create_vertex_buffer(params), _destroy_resource);

        return _try_acquire_and_add_resource<VertexBuffer::ID>(std::move(vertex_buffer), data_provider);
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    IndexBuffer::ID MeshManager::_create_index_buffer(cl7::u8string_view identifier, const IndexBuffer::Desc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateParams<IndexBuffer::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr index_buffer(_factory->create_index_buffer(params), _destroy_resource);

        return _try_acquire_and_add_resource<IndexBuffer::ID>(std::move(index_buffer), data_provider);
    }



} // namespace xl7::graphics::meshes
