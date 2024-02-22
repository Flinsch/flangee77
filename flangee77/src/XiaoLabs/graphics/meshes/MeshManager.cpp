#include "MeshManager.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    MeshManager::MeshManager(IMeshFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    MeshManager::~MeshManager()
    {
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates and acquires the specified vertex shader.
     */
    resources::ResourceID MeshManager::_create_vertex_buffer(cl7::string_view identifier, const VertexBuffer::Desc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateParams<VertexBuffer::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr vertex_buffer( _factory->create_vertex_buffer( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(vertex_buffer), data_provider );
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    resources::ResourceID MeshManager::_create_index_buffer(cl7::string_view identifier, const IndexBuffer::Desc& desc, const resources::DataProvider& data_provider)
    {
        resources::Resource::CreateParams<IndexBuffer::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr index_buffer( _factory->create_index_buffer( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(index_buffer), data_provider );
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
