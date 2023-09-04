#include "MainObjectImpl.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>

#include <dxgi.h>
#include <d3d11.h>

#pragma comment( lib, "DXGI.lib" )



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
        IDXGIFactory* dxgi_factory = nullptr;
        HRESULT hr = ::CreateDXGIFactory( __uuidof(::IDXGIFactory), (void**)&dxgi_factory );
        if ( FAILED(hr) )
        {
            LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::CreateDXGIFactory") ) );
            return false;
        }

        bool best_found = false;
        unsigned best_index = 0;
        size_t best_dedicated_video_memory = 0;

        IDXGIAdapter* dxgi_adapter = nullptr;
        for ( unsigned i = 0; dxgi_factory->EnumAdapters( i, &dxgi_adapter ) != DXGI_ERROR_NOT_FOUND; ++i )
        {
            DXGI_ADAPTER_DESC dxgi_adapter_desc;
            memset( &dxgi_adapter_desc, 0, sizeof(DXGI_ADAPTER_DESC) );
            hr = dxgi_adapter->GetDesc( &dxgi_adapter_desc );
            dxgi_adapter->Release();
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
            dxgi_factory->Release();
            LOG_ERROR( TEXT("No suitable adapter (video card) could be found.") );
            return false;
        }

        dxgi_factory->Release();

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainObjectImpl::_shutdown_without_logging_final_result()
    {
        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace video
} // namespace xl7
