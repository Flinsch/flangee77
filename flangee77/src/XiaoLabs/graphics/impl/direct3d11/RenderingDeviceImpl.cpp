#include "RenderingDeviceImpl.h"

#include "./GraphicsSystemImpl.h"
#include "./errors.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



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
        : RenderingDevice( nullptr )
        , _d3d_device( nullptr )
        , _d3d_immediate_context( nullptr )
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
        HRESULT hresult = ::D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            feature_levels,
            sizeof( feature_levels ) / sizeof( D3D_FEATURE_LEVEL ),
            D3D11_SDK_VERSION,
            _d3d_device.ReleaseAndGetAddressOf(),
            &_d3d_feature_level,
            _d3d_immediate_context.ReleaseAndGetAddressOf() );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("::D3D11CreateDevice") ) );
            LOG_ERROR( TEXT("The Direct3D 11 device interface could not be created.") );
            return false;
        }

        // Check whether Direct3D 11 is even supported!
        if ( _d3d_feature_level < D3D_FEATURE_LEVEL_11_0 )
        {
            LOG_WARNING( TEXT("Direct3D 11 is not supported.") );
        }

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

        wrl::ComPtr<IDXGIDevice> dxgi_device;
        hresult = _d3d_device.As( &dxgi_device );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::QueryInterface") ) );
            LOG_ERROR( TEXT("The DXGI device interface could not be requested.") );
            return false;
        }

        wrl::ComPtr<IDXGIAdapter> dxgi_adapter;
        hresult = dxgi_device->GetAdapter( dxgi_adapter.ReleaseAndGetAddressOf() );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGIDevice::GetAdapter") ) );
            LOG_ERROR( TEXT("The DXGI adapter interface could not be requested.") );
            return false;
        }

        DXGI_ADAPTER_DESC adapter_desc;
        memset( &adapter_desc, 0, sizeof(DXGI_ADAPTER_DESC) );
        hresult = dxgi_adapter->GetDesc( &adapter_desc );
        if ( FAILED(hresult) )
        {
            LOG_WARNING( errors::dxgi_result( hresult, TEXT("IDXGIAdapter::GetDesc") ) );
        }

        // Adopt the available
        // video memory composition.
        capabilities.memory.dedicated_video_memory = adapter_desc.DedicatedVideoMemory;
        capabilities.memory.dedicated_system_memory = adapter_desc.DedicatedSystemMemory;
        capabilities.memory.shared_system_memory = adapter_desc.SharedSystemMemory;

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



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
