#ifndef XL7_GRAPHICS_MESHES_MESHBUFFERUPDATER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFERUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./MeshBufferDesc.h"
#include "./MeshBufferWrite.h"
#include "./DirtyRange.h"



namespace xl7::graphics::meshes {



class MeshBufferUpdater
    : public resources::ResourceUpdaterBase<MeshBufferDesc, DirtyRange>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a contiguous range of elements in the mesh buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the element stride, ensuring that only whole elements are written.
     *
     * On success, the corresponding element range is marked as "dirty".
     */
    bool write(const MeshBufferWrite& write);

};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFERUPDATER_H
