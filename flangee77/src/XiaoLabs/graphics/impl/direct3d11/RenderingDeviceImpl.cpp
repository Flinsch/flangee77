#include "RenderingDeviceImpl.h"

#include "./GraphicsSystemImpl.h"
#include "./ResourceFactoryImpl.h"
#include "./errors.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/util.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    RenderingDeviceImpl::RenderingDeviceImpl()
        : RenderingDevice( std::unique_ptr<IResourceFactory>( ResourceFactoryImpl::Attorney::create() ) )
        , _d3d_feature_level( D3D_FEATURE_LEVEL_1_0_CORE )
    {
    }



    // #############################################################################
    // RenderingDevice Implementations
    // #############################################################################

    /**
     * Initializes the rendering device and determines the capabilities (as far as
     * they can be determined).
     */
    bool RenderingDeviceImpl::_init_impl(Capabilities& capabilities)
    {
        unsigned flags = 0;
#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
        };

        // (Try to) create the hardware-based
        // Direct3D 11 device interface and the
        // (immediate) device context interface.
        wrl::ComPtr<ID3D11Device> d3d_device;
        wrl::ComPtr<ID3D11DeviceContext> d3d_immediate_context;
        HRESULT hresult = ::D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            feature_levels,
            sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL),
            D3D11_SDK_VERSION,
            &d3d_device,
            &_d3d_feature_level,
            &d3d_immediate_context );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("::D3D11CreateDevice") ) );
            LOG_ERROR( TEXT("The Direct3D 11 device interface could not be created.") );
            return false;
        }

        // Check whether Direct3D 11 is even supported!
        if ( _d3d_feature_level < D3D_FEATURE_LEVEL_11_0 )
            LOG_WARNING( TEXT("Direct3D 11 is not supported.") );

        // Try to access extended interface versions.
        d3d_device.As( &_d3d_device );
        d3d_immediate_context.As( &_d3d_immediate_context );

        // Determine the supported shader versions
        // based on a hard-coded feature level mapping.
        switch ( _d3d_feature_level )
        {
        case D3D_FEATURE_LEVEL_1_0_CORE:
            capabilities.shaders.vertex_shader_version = { 0, 0 };
            capabilities.shaders.pixel_shader_version = { 0, 0 };
            break;
        case D3D_FEATURE_LEVEL_9_1:
            capabilities.shaders.vertex_shader_version = { 2, 0, 1 };
            capabilities.shaders.pixel_shader_version = { 2, 0, 1 };
            break;
        case D3D_FEATURE_LEVEL_9_2:
            capabilities.shaders.vertex_shader_version = { 2, 0, 2 };
            capabilities.shaders.pixel_shader_version = { 2, 0, 2 };
            break;
        case D3D_FEATURE_LEVEL_9_3:
            capabilities.shaders.vertex_shader_version = { 3, 0 };
            capabilities.shaders.pixel_shader_version = { 3, 0 };
            break;
        case D3D_FEATURE_LEVEL_10_0:
            capabilities.shaders.vertex_shader_version = { 4, 0 };
            capabilities.shaders.pixel_shader_version = { 4, 0 };
            break;
        case D3D_FEATURE_LEVEL_10_1:
            capabilities.shaders.vertex_shader_version = { 4, 1 };
            capabilities.shaders.pixel_shader_version = { 4, 1 };
            break;
        case D3D_FEATURE_LEVEL_11_0:
            capabilities.shaders.vertex_shader_version = { 5, 0 };
            capabilities.shaders.pixel_shader_version = { 5, 0 };
            break;
        case D3D_FEATURE_LEVEL_11_1:
            capabilities.shaders.vertex_shader_version = { 5, 0 };
            capabilities.shaders.pixel_shader_version = { 5, 0 };
            break;
        default:
            assert( false );
        }

        // Query the DXGI device interface.
        wrl::ComPtr<IDXGIDeviceN> dxgi_device;
        hresult = _d3d_device.As( &dxgi_device );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::QueryInterface") ) );
            LOG_ERROR( TEXT("The DXGI device interface could not be requested.") );
            return false;
        }

        // Get the DXGI adapter interface.
        wrl::ComPtr<IDXGIAdapterN> dxgi_adapter;
        hresult = dxgi_device->GetAdapter( &dxgi_adapter );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGIDevice::GetAdapter") ) );
            LOG_ERROR( TEXT("The DXGI adapter interface could not be requested.") );
            return false;
        }

        // Get the DXGI factory interface.
        wrl::ComPtr<IDXGIFactoryN> dxgi_factory;
        hresult = dxgi_adapter->GetParent( __uuidof(IDXGIFactoryN), &dxgi_factory );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGIAdapter::GetParent") ) );
            LOG_ERROR( TEXT("The DXGI factory interface could not be requested.") );
            return false;
        }

        // Cache the main window's display mode.
        const MainWindow::DisplayMode window_display_mode = MainWindow::instance().get_display_mode();
        const bool fullscreen = window_display_mode == MainWindow::DisplayMode::Fullscreen;

        // "Calculate" the back buffer size.
        const unsigned back_buffer_width = cl7::util::coalesce( GraphicsSystem::instance().get_config().video.display_mode.width, MainWindow::instance().get_width() );
        const unsigned back_buffer_height = cl7::util::coalesce( GraphicsSystem::instance().get_config().video.display_mode.height, MainWindow::instance().get_height() );

        // Fill the swap chain description structure.
        DXGI_SWAP_CHAIN_DESCn dxgi_swap_chain_desc;
        ::memset( &dxgi_swap_chain_desc, 0, sizeof(dxgi_swap_chain_desc) );
        dxgi_swap_chain_desc.BufferDesc.Width = back_buffer_width;
        dxgi_swap_chain_desc.BufferDesc.Height = back_buffer_height;
        dxgi_swap_chain_desc.BufferDesc.RefreshRate.Numerator = fullscreen ? GraphicsSystem::instance().get_config().video.display_mode.refresh_rate : 0;
        dxgi_swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        dxgi_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxgi_swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        dxgi_swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        dxgi_swap_chain_desc.SampleDesc.Quality = 0;
        dxgi_swap_chain_desc.SampleDesc.Count = 1;
        dxgi_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgi_swap_chain_desc.BufferCount = 1;
        dxgi_swap_chain_desc.OutputWindow = MainWindow::instance().get_handle();
        dxgi_swap_chain_desc.Windowed = !fullscreen;
        dxgi_swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        dxgi_swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // Create the swap chain.
        hresult = dxgi_factory->CreateSwapChain( _d3d_device.Get(), &dxgi_swap_chain_desc, &_dxgi_swap_chain );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGIFactory::CreateSwapChain") ) );
            LOG_ERROR( TEXT("The DXGI swap chain could not be created.") );
            return false;
        }

        // Ask for the adapter description.
        DXGI_ADAPTER_DESC dxgi_adapter_desc;
        memset( &dxgi_adapter_desc, 0, sizeof(DXGI_ADAPTER_DESC) );
        hresult = dxgi_adapter->GetDesc( &dxgi_adapter_desc );
        if ( FAILED(hresult) )
            LOG_WARNING( errors::dxgi_result( hresult, TEXT("IDXGIAdapter::GetDesc") ) );

        // Adopt the available
        // video memory composition.
        capabilities.memory.dedicated_video_memory = dxgi_adapter_desc.DedicatedVideoMemory;
        capabilities.memory.dedicated_system_memory = dxgi_adapter_desc.DedicatedSystemMemory;
        capabilities.memory.shared_system_memory = dxgi_adapter_desc.SharedSystemMemory;

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool RenderingDeviceImpl::_shutdown_impl()
    {
        // Release the (immediate) device context interface.
        _d3d_immediate_context.Reset();

        // Release the Direct3D 11 device interface.
        _d3d_device.Reset();

        return true;
    }

    /**
     * Checks whether the device is lost. If so, true is returned.
     */
    bool RenderingDeviceImpl::_check_device_lost_impl()
    {
        

        return false;
    }

    /**
     * Tries to reset/reinitialize the device after it has been lost. If the device
     * has been restored to an operational state, true is returned.
     */
    bool RenderingDeviceImpl::_handle_device_lost_impl()
    {
        

        return true;
    }


    /**
     * Begins a scene.
     */
    bool RenderingDeviceImpl::_begin_scene_impl()
    {
        // Nothing to do here?

        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingDeviceImpl::_end_scene_impl()
    {
        // Nothing to do here?

        return true;
    }

    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool RenderingDeviceImpl::_present_impl()
    {
        HRESULT hresult = _dxgi_swap_chain->Present( 1, 0 );

        if ( hresult == DXGI_ERROR_DEVICE_REMOVED || hresult == DXGI_ERROR_DEVICE_RESET )
        {
            _notify_device_lost();
            return false;
        }

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGISwapChain::Present") ) );
            return false;
        }

        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
