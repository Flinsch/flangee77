#pragma once
#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#include "./MeshBuffer.h"

#include "./IndexType.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class IndexBuffer;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;



class MeshManager;



class IndexBuffer
    : public MeshBuffer
{

public:
    struct Desc
        : public MeshBuffer::Desc
    {
        /** The type/format (16 or 32 bits) of the index buffer. */
        IndexType index_type;
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

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the index buffer.
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
    virtual cl7::string_view get_type_string() const override { return TEXT("index buffer"); }

}; // class IndexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
