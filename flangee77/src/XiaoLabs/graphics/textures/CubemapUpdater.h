#ifndef XL7_GRAPHICS_TEXTURES_CUBEMAPUPDATER_H
#define XL7_GRAPHICS_TEXTURES_CUBEMAPUPDATER_H
#include "../../resources/ResourceUpdaterBase.h"

#include "./CubemapDesc.h"
#include "./CubemapWrite.h"
#include "./DirtyLayerRects.h"



namespace xl7::graphics::textures {



class CubemapUpdater
    : public resources::ResourceUpdaterBase<CubemapDesc, DirtyLayerRects>
{
public:
    using ResourceUpdaterBase::ResourceUpdaterBase;



    /**
     * Updates a rectangular region of a cubemap face, and returns true if the
     * update succeeded, false otherwise.
     *
     * The input data must match the size of the specified region, taking into
     * account the texture format and row pitch requirements, ensuring that only
     * whole pixels are written.
     *
     * On success, the updated region is marked as "dirty".
     */
    bool write(const CubemapWrite& write);

};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_CUBEMAPUPDATER_H
