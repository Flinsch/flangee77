#include "Texture2D.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture2D::Texture2D(const CreateParams<Desc>& params)
        : Texture( Type::Texture2D, { params.manager, params.id, params.identifier, params.desc }, 1 )
        , _desc( params.desc )
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
