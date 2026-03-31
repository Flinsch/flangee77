#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DUPDATER_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./Texture2DDesc.h"
#include "./Texture2DWrite.h"
#include "./DirtyRect.h"



namespace xl7::graphics::textures {



class Texture2DUpdater
    : public resources::ResourceUpdaterBase<Texture2DDesc, DirtyRect>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a rectangular region of a 2D texture, and returns true if the update
     * succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row pitch requirements, ensuring that only
     * whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool write(const Texture2DWrite& write);

};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DUPDATER_H
