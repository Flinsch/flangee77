#include "ColorRenderTarget.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ColorRenderTarget::ColorRenderTarget(const CreateParams<Desc>& params)
        : RenderTarget( Type::ColorRenderTarget, params )
    {
    }



} // namespace surfaces
} // namespace graphics
} // namespace xl7
