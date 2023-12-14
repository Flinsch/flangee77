#include "TextureManager.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    TextureManager::TextureManager(ITextureFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    TextureManager::~TextureManager()
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
