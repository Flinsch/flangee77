#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#include "../../Resource.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;



class MeshManager;



class VertexBuffer
    : public Resource
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit VertexBuffer(MeshManager* manager);

    /**
     * Explicit constructor.
     */
    VertexBuffer(MeshManager* manager, const cl7::string& identifier);

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

}; // class VertexBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
