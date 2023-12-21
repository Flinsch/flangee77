#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#include "../../Resource.h"

#include "./VertexLayout.h"
#include "./Topology.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;



class MeshManager;



class VertexBuffer
    : public Resource
{

public:
    struct Desc
    {
        /** The layout specification of the vertices. */
        VertexLayout vertex_layout;
        /** The size of each vertex, in bytes. */
        unsigned vertex_stride;
        /** The number of vertices within the vertex buffer. */
        unsigned vertex_count;
        /** The topology of the primitives that the vertex buffer represents. */
        Topology topology;

    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexBuffer(MeshManager* manager, const cl7::string& identifier, const Desc& desc);

    /**
     * Destructor.
     */
    virtual ~VertexBuffer() = default;

private:
    /** Default constructor. */
    VertexBuffer() = delete;
    /** Copy constructor. */
    VertexBuffer(const VertexBuffer&) = delete;
    /** Copy assignment operator. */
    VertexBuffer& operator = (const VertexBuffer&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /** The description of the vertex buffer. */
    const Desc desc;

    /**
     * The size of this vertex buffer, in bytes.
     */
    const unsigned size;

private:

}; // class VertexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
