#pragma once
#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#include "../../Resource.h"

#include "./IndexType.h"
#include "./Topology.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class IndexBuffer;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;



class MeshManager;



class IndexBuffer
    : public Resource
{

public:
    struct Desc
    {
        /** The type/format (16 or 32 bits) of the index buffer. */
        IndexType index_type;
        /** The number of indices within the index buffer. */
        unsigned index_count;
        /** The topology of the primitives that the index buffer represents. */
        Topology topology;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    IndexBuffer(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~IndexBuffer() = default;

private:
    /** Default constructor. */
    IndexBuffer() = delete;
    /** Copy constructor. */
    IndexBuffer(const IndexBuffer&) = delete;
    /** Copy assignment operator. */
    IndexBuffer& operator = (const IndexBuffer&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the index buffer.
     */
    const Desc _desc;

    /**
     * The size of each index, in bytes.
     */
    const unsigned _stride;

    /**
     * The size of this index buffer, in bytes.
     */
    const unsigned _size;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the index buffer.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns the size of each index, in bytes.
     */
    unsigned get_stride() const { return _stride; }

    /**
     * Returns the size of this index buffer, in bytes.
     */
    unsigned get_size() const { return _size; }

public:

}; // class IndexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
