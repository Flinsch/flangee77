#include "BlendState.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    BlendState::BlendState(const CreateParams<Desc>& params)
        : AbstractState( params )
        , _desc( params.desc )
    {
    }



} // namespace states
} // namespace graphics
} // namespace xl7
