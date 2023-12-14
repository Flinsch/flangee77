#include "MeshManager.h"



namespace xl7 {
namespace graphics {
namespace meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    MeshManager::MeshManager(IMeshFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    MeshManager::~MeshManager()
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
