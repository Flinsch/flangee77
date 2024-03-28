#include "SamplerState.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SamplerState::SamplerState(const CreateParams<Desc>& params)
        : AbstractState( params )
        , _desc( params.desc )
    {
    }



} // namespace states
} // namespace graphics
} // namespace xl7
