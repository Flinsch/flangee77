#include "RasterizerState.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RasterizerState::RasterizerState(const CreateParams<Desc>& params)
        : AbstractState( params )
        , _desc( params.desc )
    {
    }



} // namespace states
} // namespace graphics
} // namespace xl7
