#include "Texture2DArray.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture2DArray::Texture2DArray(const CreateParams<Desc>& params)
        : Texture( Type::Texture2DArray, { params.manager, params.id, params.identifier, params.desc } )
        , _desc( params.desc )
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
