#include "MainObjectImpl.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>

#pragma comment( lib, "d3d9.lib" )



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
        : _d3d_interface()
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
        if ( !_create_main_interface() )
            return false;

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainObjectImpl::_shutdown_without_logging_final_result()
    {
        _release_main_interface();

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the Direct3D 9 main interface.
     */
    bool MainObjectImpl::_create_main_interface()
    {
        if ( _d3d_interface )
        {
            LOG_WARNING( TEXT("The Direct3D 9 main interface has already been created.") );
            return true;
        }

        _d3d_interface = ::Direct3DCreate9( D3D_SDK_VERSION );

        if ( !_d3d_interface )
        {
            LOG_ERROR( TEXT("The Direct3D 9 main interface could not be created.") );
            return false;
        }

        LOG( TEXT("The Direct3D 9 main interface has been created.") );
        return true;
    }

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool MainObjectImpl::_release_main_interface()
    {
        _d3d_interface.Reset();

        LOG( TEXT("The Direct3D 9 main interface has been released.") );
        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace video
} // namespace xl7
