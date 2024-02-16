#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#include "./MeshBuffer.h"

#include "./VertexLayout.h"
#include "./VertexDataProvider.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class MeshManager;



class VertexBuffer
    : public MeshBuffer
{

public:
    struct Desc
        : public MeshBuffer::Desc
    {
        /** The size of each vertex, in bytes. */
        unsigned stride;
        /** The layout specification of the vertices. */
        VertexLayout vertex_layout;
        /** The number of instances to draw using the same per-instance data before advancing in the buffer by one element, or simply 0 if the buffer contains "regular" per-vertex data. */
        unsigned instance_data_step_rate;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexBuffer(const CreateParams<Desc>& params);

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
protected:
    /**
     * The descriptor of the vertex buffer.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the vertex buffer.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("vertex buffer"); }

}; // class VertexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
