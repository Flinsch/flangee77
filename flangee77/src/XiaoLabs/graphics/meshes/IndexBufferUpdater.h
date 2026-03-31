#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFERUPDATER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFERUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./IndexBufferDesc.h"
#include "./IndexBufferWrite.h"
#include "./DirtyRange.h"



namespace xl7::graphics::meshes {



class IndexBufferUpdater
    : public resources::ResourceUpdaterBase<IndexBufferDesc, DirtyRange>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a contiguous range of indices in the index buffer, and returns true
     * if the update succeeded, false otherwise.
     *
     * The input data must match the size of the specified range, taking into
     * account the index format, ensuring that only whole indices are written.
     *
     * On success, the corresponding index range is marked as "dirty".
     */
    bool write(const IndexBufferWrite& write);

};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFERUPDATER_H
