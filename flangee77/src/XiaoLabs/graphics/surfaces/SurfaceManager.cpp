#include "SurfaceManager.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SurfaceManager::SurfaceManager(ISurfaceFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    SurfaceManager::~SurfaceManager()
    {
    }



} // namespace surfaces
} // namespace graphics
} // namespace xl7
