#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFERUPDATER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFERUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./VertexBufferDesc.h"
#include "./VertexBufferWrite.h"
#include "./DirtyRange.h"



namespace xl7::graphics::meshes {



class VertexBufferUpdater
    : public resources::ResourceUpdaterBase<VertexBufferDesc, DirtyRange>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a contiguous range of vertices in the vertex buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the vertex stride, ensuring that only whole vertices are written.
     *
     * On success, the corresponding vertex range is marked as "dirty".
     */
    bool write(const VertexBufferWrite& write);

};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFERUPDATER_H
