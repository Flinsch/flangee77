#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHMANAGER_H
#define XL7_GRAPHICS_MESHES_MESHMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./IMeshFactory.h"



namespace xl7::graphics {
    class RenderingDevice;
namespace meshes {



class MeshManager final // NOLINT(*-virtual-class-destructor)
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static MeshManager* create(IMeshFactory* factory) { return new MeshManager(factory); }
        static void destroy(MeshManager* manager) { delete manager; }
        friend class xl7::graphics::RenderingDevice;
    };



    MeshManager() = delete;

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator = (const MeshManager&) = delete;
    MeshManager(MeshManager&&) = delete;
    MeshManager& operator = (MeshManager&&) = delete;



    /**
     * Creates and acquires the specified vertex shader.
     */
    template <class TVertex>
    resources::ResourceID create_vertex_buffer(cl7::astring_view identifier, const VertexBuffer::Desc& desc, const VertexDataProvider<TVertex>& vertex_data_provider)
    {
        return _create_vertex_buffer(identifier, desc, vertex_data_provider);
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    template <class TIndex>
    resources::ResourceID create_index_buffer(cl7::astring_view identifier, const IndexBuffer::Desc& desc, const IndexDataProvider<TIndex>& index_data_provider)
    {
        return _create_index_buffer(identifier, desc, index_data_provider);
    }



protected:
    MeshManager(IMeshFactory* factory) : _factory(factory) {}
    ~MeshManager() override = default;



private:

    /**
     * Creates and acquires the specified vertex shader.
     */
    resources::ResourceID _create_vertex_buffer(cl7::astring_view identifier, const VertexBuffer::Desc& desc, const resources::DataProvider& data_provider);

    /**
     * Creates and acquires the specified index buffer.
     */
    resources::ResourceID _create_index_buffer(cl7::astring_view identifier, const IndexBuffer::Desc& desc, const resources::DataProvider& data_provider);



    /**
     * The mesh factory.
     */
    IMeshFactory* const _factory;

}; // class MeshManager



} // namespace meshes
} // namespace xl7::graphics

#endif // XL7_GRAPHICS_MESHES_MESHMANAGER_H
