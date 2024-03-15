#include "Texture3D.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture3D::Texture3D(const CreateParams<Desc>& params)
        : Texture( Type::Texture3D, { params.manager, params.id, params.identifier, params.desc }, params.desc.depth, 1 )
        , _desc( params.desc )
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
