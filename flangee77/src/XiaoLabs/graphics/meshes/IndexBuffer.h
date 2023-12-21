#pragma once
#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#include "../../Resource.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class IndexBuffer;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;



class MeshManager;



class IndexBuffer
    : public Resource
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit IndexBuffer(MeshManager* manager);

    /**
     * Explicit constructor.
     */
    IndexBuffer(MeshManager* manager, const cl7::string& identifier);

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

}; // class IndexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
