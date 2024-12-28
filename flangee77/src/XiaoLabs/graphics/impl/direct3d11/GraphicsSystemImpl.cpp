#include "GraphicsSystemImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d11 {



    // #############################################################################
    // GraphicsSystem Implementations
    // #############################################################################

    /**
     * Performs preliminary initialization steps so that the rendering device can be
     * created afterward.
     */
    bool GraphicsSystemImpl::_init_before_rendering_device_impl()
    {
        return _create_dxgi_factory();
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
     * fully initializing it so that it can be initialized afterward.
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
        if (_dxgi_factory)
        {
            LOG_WARNING(u8"The DXGI factory interface has already been created.");
            return true;
        }

        HRESULT hresult = ::CreateDXGIFactory1(__uuidof(IDXGIFactoryN), &_dxgi_factory);
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::dxgi_result(hresult, u8"::CreateDXGIFactory1"));
            return false;
        }

        /*bool best_found = false;
        unsigned best_index = 0;
        size_t best_dedicated_video_memory = 0;

        wrl::ComPtr<IDXGIAdapter> dxgi_adapter;
        for (unsigned i = 0; _dxgi_factory->EnumAdapters(i, &dxgi_adapter) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            DXGI_ADAPTER_DESC dxgi_adapter_desc = {};
            hresult = dxgi_adapter->GetDesc(&dxgi_adapter_desc);
            if (FAILED(hresult))
            {
                LOG_WARNING(cl7::errors::system_result(hresult, u8"IDXGIAdapter::GetDesc"));
                continue;
            }

            if (!best_found || dxgi_adapter_desc.DedicatedVideoMemory > best_dedicated_video_memory)
            {
                best_dedicated_video_memory = dxgi_adapter_desc.DedicatedVideoMemory;
                best_index = i;
                best_found = true;
            }
        } // for each enumerated adapter

        if (!best_found)
        {
            LOG_ERROR(u8"No suitable adapter (video card) could be found.");
            return false;
        }*/

        LOG(u8"The DXGI factory interface has been created.");
        return true;
    }

    /**
     * Releases the DXGI factory interface.
     */
    bool GraphicsSystemImpl::_release_dxgi_factory()
    {
        _dxgi_factory.Reset();

        LOG(u8"The DXGI factory interface has been released.");
        return true;
    }



} // namespace xl7::graphics::impl::direct3d11
