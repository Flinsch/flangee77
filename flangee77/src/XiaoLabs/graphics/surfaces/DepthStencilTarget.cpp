#include "DepthStencilTarget.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    DepthStencilTarget::DepthStencilTarget(const CreateParams<Desc>& params)
        : Surface( Type::DepthStencilTarget, params )
    {
    }



} // namespace surfaces
} // namespace graphics
} // namespace xl7
