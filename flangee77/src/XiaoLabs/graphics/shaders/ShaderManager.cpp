#include "ShaderManager.h"



namespace xl7 {
namespace graphics {
namespace shaders {


    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ShaderManager::ShaderManager(IShaderFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    ShaderManager::~ShaderManager()
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
