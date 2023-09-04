#include "MainObject.h"

#if XL7_VIDEO_IMPL == XL7_VIDEO_IMPL_DIRECT3D9
#define XL7_VIDEO_IMPL_NAME "Direct3D 9"
#include "./impl/direct3d9/MainObjectImpl.h"
typedef xl7::video::impl::direct3d9::MainObjectImpl MainObjectImpl;

#elif XL7_VIDEO_IMPL == XL7_VIDEO_IMPL_DIRECT3D11
#define XL7_VIDEO_IMPL_NAME "Direct3D 11"
#include "./impl/direct3d11/MainObjectImpl.h"
typedef xl7::video::impl::direct3d11::MainObjectImpl MainObjectImpl;

#else
#ifdef XL7_VIDEO_IMPL
#error Bad XL7_VIDEO_IMPL definition.
#else
#error Missing XL7_VIDEO_IMPL definition.
#endif
#endif

#pragma message( "The XiaoLabs video implementation is based on " XL7_VIDEO_IMPL_NAME "." )

#include <CoreLabs/logging.h>



namespace xl7 {
namespace video {



    // #############################################################################
    // Factory Function
    // #############################################################################

    MainObject* MainObject::factory_func()
    {
        return new MainObjectImpl();
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool MainObject::_init()
    {
        if ( _init_without_logging_final_result() )
        {
            LOG_SUCCESS( TEXT("The " XL7_VIDEO_IMPL_NAME " main object was successfully initialized.") );
            return true;
        }
        else
        {
            LOG_ERROR( TEXT("The " XL7_VIDEO_IMPL_NAME " main object could not be initialized..") );
            return false;
        }
    }

    /**
     * De-initializes the component.
     */
    bool MainObject::_shutdown()
    {
        if ( _shutdown_without_logging_final_result() )
        {
            LOG_SUCCESS( TEXT("The " XL7_VIDEO_IMPL_NAME " main object was shut down successfully.") );
            return true;
        }
        else
        {
            LOG_WARNING( TEXT("The " XL7_VIDEO_IMPL_NAME " main object could not be shut down correctly.") );
            return false;
        }
    }



} // namespace video
} // namespace xl7
