#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3DUPDATER_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3DUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./Texture3DDesc.h"
#include "./Texture3DWrite.h"
#include "./DirtyBox.h"



namespace xl7::graphics::textures {



class Texture3DUpdater
    : public resources::ResourceUpdaterBase<Texture3DDesc, DirtyBox>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a cubic region of a 3D texture, and returns true if the update
     * succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row/slice pitch requirements, ensuring that
     * only whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool write(const Texture3DWrite& write);

};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3DUPDATER_H
