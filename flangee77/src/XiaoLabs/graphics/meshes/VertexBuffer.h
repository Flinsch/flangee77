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
        /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
        ResourceUsage usage;
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

    /**
     * The size of each vertex, in bytes.
     */
    const unsigned _stride;

    /**
     * The size of this vertex buffer, in bytes.
     */
    const unsigned _size;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the vertex buffer.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns the size of each vertex, in bytes.
     */
    unsigned get_stride() const { return _stride; }

    /**
     * Returns the size of this vertex buffer, in bytes.
     */
    unsigned get_size() const { return _size; }

public:



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     */
    bool update(const DataProvider& data_provider);



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_impl(const DataProvider& data_provider) override;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("vertex buffer"); }



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const DataProvider& data_provider, bool discard, bool no_overwrite) = 0;

}; // class VertexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
