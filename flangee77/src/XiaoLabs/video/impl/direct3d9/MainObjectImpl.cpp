#include "MainObjectImpl.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>



namespace xl7 {
namespace video {
namespace impl {
namespace direct3d9 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    MainObjectImpl::MainObjectImpl(void)
    {
    }



    // #############################################################################
    // MainObject Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool MainObjectImpl::_init_without_logging_final_result()
    {
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainObjectImpl::_shutdown_without_logging_final_result()
    {
        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace video
} // namespace xl7
