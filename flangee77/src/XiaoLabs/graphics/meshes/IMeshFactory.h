#ifndef XL7_GRAPHICS_MESHES_IMESHFACTORY_H
#define XL7_GRAPHICS_MESHES_IMESHFACTORY_H

#include "./VertexBuffer.h"
#include "./IndexBuffer.h"



namespace xl7::graphics::meshes {



class MeshManager;



class IMeshFactory
{
public:
    virtual ~IMeshFactory() = default;

    virtual VertexBuffer* create_vertex_buffer(const resources::Resource::CreateContext& ctx, const VertexBufferDesc& desc) = 0;
    virtual IndexBuffer* create_index_buffer(const resources::Resource::CreateContext& ctx, const IndexBufferDesc& desc) = 0;

}; // class IMeshFactory



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_IMESHFACTORY_H
