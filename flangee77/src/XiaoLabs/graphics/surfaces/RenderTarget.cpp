#include "RenderTarget.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderTarget::RenderTarget(Type type, const CreateParams<Desc>& params)
        : Surface( Type::ColorRenderTarget, params )
        , _default_viewport{ 0, 0, params.desc.width, params.desc.height, 0.0f, 1.0f }
    {
    }



} // namespace surfaces
} // namespace graphics
} // namespace xl7
