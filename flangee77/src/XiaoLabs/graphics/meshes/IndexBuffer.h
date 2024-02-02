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



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Updates the contents of this index buffer (unless it is immutable).
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
    virtual cl7::string_view get_type_string() const override { return TEXT("index buffer"); }



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Updates the contents of this index buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const DataProvider& data_provider, bool discard, bool no_overwrite) = 0;

}; // class IndexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
