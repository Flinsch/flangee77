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
    // Methods
    // #############################################################################

    /**
     * Creates and acquires the specified vertex shader.
     */
    VertexBufferPtr MeshManager::create_vertex_buffer(const cl7::string_view& identifier, const VertexBuffer::Desc& desc, const VertexBuffer::DataProvider& data_provider)
    {
        Resource::CreateParams<VertexBuffer::Desc> params{ this, identifier, desc };

        VertexBufferPtr vertex_buffer( _factory->create_vertex_buffer( params ), _destroy_resource );

        return _try_acquire_and_add_resource( vertex_buffer, data_provider );
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    IndexBufferPtr MeshManager::create_index_buffer(const cl7::string_view& identifier, const IndexBuffer::Desc& desc, const IndexBuffer::DataProvider& data_provider)
    {
        Resource::CreateParams<IndexBuffer::Desc> params{ this, identifier, desc };

        IndexBufferPtr index_buffer( _factory->create_index_buffer( params ), _destroy_resource );

        return _try_acquire_and_add_resource( index_buffer, data_provider );
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
