#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHMANAGER_H
#define XL7_GRAPHICS_MESHES_MESHMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./IMeshFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace meshes {



class MeshManager
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static MeshManager* create(IMeshFactory* factory) { return new MeshManager( factory ); }
        static void destroy(MeshManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    MeshManager(IMeshFactory* factory);

    /**
     * Destructor.
     */
    virtual ~MeshManager();

private:
    /** Default constructor. */
    MeshManager() = delete;
    /** Copy constructor. */
    MeshManager(const MeshManager&) = delete;
    /** Copy assignment operator. */
    MeshManager& operator = (const MeshManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The mesh factory.
     */
    IMeshFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Creates and acquires the specified vertex shader.
     */
    template <class TVertex>
    resources::ResourceID create_vertex_buffer(cl7::astring_view identifier, const VertexBuffer::Desc& desc, const VertexDataProvider<TVertex>& data_provider)
    {
        return _create_vertex_buffer( identifier, desc, data_provider );
    }

    /**
     * Creates and acquires the specified index buffer.
     */
    template <class TIndex>
    resources::ResourceID create_index_buffer(cl7::astring_view identifier, const IndexBuffer::Desc& desc, const IndexDataProvider<TIndex>& data_provider)
    {
        return _create_index_buffer( identifier, desc, data_provider );
    }



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates and acquires the specified vertex shader.
     */
    resources::ResourceID _create_vertex_buffer(cl7::astring_view identifier, const VertexBuffer::Desc& desc, const resources::DataProvider& data_provider);

    /**
     * Creates and acquires the specified index buffer.
     */
    resources::ResourceID _create_index_buffer(cl7::astring_view identifier, const IndexBuffer::Desc& desc, const resources::DataProvider& data_provider);

}; // class MeshManager



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_MESHMANAGER_H
