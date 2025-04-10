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

    virtual xl7::graphics::meshes::VertexBuffer* create_vertex_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::VertexBuffer::Desc>& params) = 0;
    virtual xl7::graphics::meshes::IndexBuffer* create_index_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::IndexBuffer::Desc>& params) = 0;

}; // class IMeshFactory



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_IMESHFACTORY_H
