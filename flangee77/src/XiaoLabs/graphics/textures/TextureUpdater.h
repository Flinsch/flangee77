#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREUPDATER_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./TextureDesc.h"
#include "./TextureWrite.h"
#include "./DirtyLayerBoxes.h"



namespace xl7::graphics::textures {



class TextureUpdater
    : public resources::ResourceUpdaterBase<TextureDesc, DirtyLayerBoxes>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a region of a texture, and returns true if the update succeeded,
     * false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row/slice pitch requirements, ensuring that
     * only whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool write(const TextureWrite& write);

};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREUPDATER_H
