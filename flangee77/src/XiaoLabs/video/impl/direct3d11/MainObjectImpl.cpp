#include "MainObjectImpl.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>

#pragma comment( lib, "dxgi.lib" )



namespace xl7 {
namespace video {
namespace impl {
namespace direct3d11 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    MainObjectImpl::MainObjectImpl(void)
        : _dxgi_factory()
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
        if ( !_create_dxgi_factory() )
            return false;

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainObjectImpl::_shutdown_without_logging_final_result()
    {
        _release_dxgi_factory();

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the DXGI factory interface.
     */
    bool MainObjectImpl::_create_dxgi_factory()
    {
        if ( _dxgi_factory )
        {
            LOG_WARNING( TEXT("The DXGI factory interface has already been created.") );
            return true;
        }

        HRESULT hr = ::CreateDXGIFactory( __uuidof(::IDXGIFactory), (void**)_dxgi_factory.ReleaseAndGetAddressOf() );
        if ( FAILED(hr) )
        {
            LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::CreateDXGIFactory") ) );
            return false;
        }

        /*bool best_found = false;
        unsigned best_index = 0;
        size_t best_dedicated_video_memory = 0;

        wrl::ComPtr<IDXGIAdapter> dxgi_adapter;
        for ( unsigned i = 0; _dxgi_factory->EnumAdapters( i, dxgi_adapter.ReleaseAndGetAddressOf() ) != DXGI_ERROR_NOT_FOUND; ++i )
        {
            DXGI_ADAPTER_DESC dxgi_adapter_desc;
            memset( &dxgi_adapter_desc, 0, sizeof(DXGI_ADAPTER_DESC) );
            hr = dxgi_adapter->GetDesc( &dxgi_adapter_desc );
            if ( FAILED(hr) )
            {
                LOG_WARNING( cl7::errors::system_result( hr, TEXT("IDXGIAdapter::GetDesc") ) );
                continue;
            }

            if ( !best_found || dxgi_adapter_desc.DedicatedVideoMemory > best_dedicated_video_memory )
            {
                best_dedicated_video_memory = dxgi_adapter_desc.DedicatedVideoMemory;
                best_index = i;
                best_found = true;
            }
        } // for each enumerated adapter

        if ( !best_found )
        {
            LOG_ERROR( TEXT("No suitable adapter (video card) could be found.") );
            return false;
        }*/

        LOG( TEXT("The DXGI factory interface has been created.") );
        return true;
    }

    /**
     * Releases the DXGI factory interface.
     */
    bool MainObjectImpl::_release_dxgi_factory()
    {
        _dxgi_factory.Reset();

        LOG( TEXT("The DXGI factory interface has been released.") );
        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace video
} // namespace xl7
