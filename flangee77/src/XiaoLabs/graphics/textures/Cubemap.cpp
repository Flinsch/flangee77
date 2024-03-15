#include "Cubemap.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Cubemap::Cubemap(const CreateParams<Desc>& params)
        : Texture( Type::Cubemap, { params.manager, params.id, params.identifier, params.desc }, 1, 6 )
        , _desc( params.desc )
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
