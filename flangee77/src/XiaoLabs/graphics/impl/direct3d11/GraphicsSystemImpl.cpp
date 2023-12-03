#include "GraphicsSystemImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <CoreLabs/logging.h>

#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3d11.lib" )



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterwards.
     */
    bool GraphicsSystemImpl::_init_before_rendering_device_impl()
    {
        if ( !_create_dxgi_factory() )
            return false;

        return true;
    }

    /**
     * Handles any remaining cleanup actions after the rendering device has been
     * destroyed.
     */
    bool GraphicsSystemImpl::_shutdown_after_rendering_device_impl()
    {
        _release_dxgi_factory();

        return true;
    }

    /**
     * Creates the rendering device (and all of its manager objects), but without
     * fully initializing it so that it can be initialized afterwards.
     */
    RenderingDevice* GraphicsSystemImpl::_rendering_device_factory_impl()
    {
        return new RenderingDeviceImpl();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the DXGI factory interface.
     */
    bool GraphicsSystemImpl::_create_dxgi_factory()
    {
        if ( _dxgi_factory )
        {
            LOG_WARNING( TEXT("The DXGI factory interface has already been created.") );
            return true;
        }

        HRESULT hr = ::CreateDXGIFactory( __uuidof(::IDXGIFactory), (void**)_dxgi_factory.ReleaseAndGetAddressOf() );
        if ( FAILED(hr) )
        {
            LOG_ERROR( errors::dxgi_result( ::GetLastError(), TEXT("::CreateDXGIFactory") ) );
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
    bool GraphicsSystemImpl::_release_dxgi_factory()
    {
        _dxgi_factory.Reset();

        LOG( TEXT("The DXGI factory interface has been released.") );
        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
