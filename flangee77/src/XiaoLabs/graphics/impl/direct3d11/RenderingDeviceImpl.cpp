#include "RenderingDeviceImpl.h"

#include "./GraphicsSystemImpl.h"
#include "./RenderingContextImpl.h"
#include "./ResourceFactoryImpl.h"
#include "./errors.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/utilities.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    static LPCSTR _d3d_semantic_name_from(xl7::graphics::meshes::VertexLayout::Semantic semantic)
    {
        switch ( semantic )
        {
        case xl7::graphics::meshes::VertexLayout::Semantic::POSITION:       return "POSITION";
        case xl7::graphics::meshes::VertexLayout::Semantic::POSITIONT:      return "POSITIONT";
        case xl7::graphics::meshes::VertexLayout::Semantic::NORMAL:         return "NORMAL";
        case xl7::graphics::meshes::VertexLayout::Semantic::TANGENT:        return "TANGENT";
        case xl7::graphics::meshes::VertexLayout::Semantic::BINORMAL:       return "BINORMAL";
        case xl7::graphics::meshes::VertexLayout::Semantic::COLOR:          return "COLOR";
        case xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD:       return "TEXCOORD";
        case xl7::graphics::meshes::VertexLayout::Semantic::BLENDINDICES:   return "BLENDINDICES";
        case xl7::graphics::meshes::VertexLayout::Semantic::BLENDWEIGHT:    return "BLENDWEIGHT";
        case xl7::graphics::meshes::VertexLayout::Semantic::PSIZE:          return "PSIZE";
        default:
            assert( false );
        }

        return "";
    }

    static DXGI_FORMAT _dxgi_format_from(xl7::graphics::meshes::VertexLayout::DataType data_type)
    {
        switch ( data_type )
        {
        case xl7::graphics::meshes::VertexLayout::DataType::FLOAT1:     return DXGI_FORMAT_R32_FLOAT;
        case xl7::graphics::meshes::VertexLayout::DataType::FLOAT2:     return DXGI_FORMAT_R32G32_FLOAT;
        case xl7::graphics::meshes::VertexLayout::DataType::FLOAT3:     return DXGI_FORMAT_R32G32B32_FLOAT;
        case xl7::graphics::meshes::VertexLayout::DataType::FLOAT4:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case xl7::graphics::meshes::VertexLayout::DataType::COLOR:      return DXGI_FORMAT_R8G8B8A8_UINT;
        case xl7::graphics::meshes::VertexLayout::DataType::UBYTE4:     return DXGI_FORMAT_R8G8B8A8_UINT;
        default:
            assert( false );
        }

        return DXGI_FORMAT_UNKNOWN;
    }



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

        // Adopt other capabilities.
        capabilities.max_simultaneous_render_target_count = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
        capabilities.max_concurrent_vertex_stream_count = D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;

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
        const unsigned back_buffer_width = cl7::utilities::coalesce( GraphicsSystem::instance().get_config().video.display_mode.width, MainWindow::instance().get_width() );
        const unsigned back_buffer_height = cl7::utilities::coalesce( GraphicsSystem::instance().get_config().video.display_mode.height, MainWindow::instance().get_height() );

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

        // Query the (primary) back buffer in order to ...
        wrl::ComPtr<ID3D11Texture2D> d3d_back_buffer;
        hresult = _dxgi_swap_chain->GetBuffer( 0, __uuidof(ID3D11Texture2D), &d3d_back_buffer );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("IDXGISwapChain::GetBuffer") ) );
            LOG_ERROR( TEXT("The DXGI swap chain's primary back buffer could not be accessed.") );
            return false;
        }

        // ... create the (standard) render target view interface, ...
        hresult = _d3d_device->CreateRenderTargetView( d3d_back_buffer.Get(), nullptr, &_d3d_render_target_view );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("ID3D11Device::CreateRenderTargetView") ) );
            LOG_ERROR( TEXT("The Direct3D 11 (standard) render target view interface could not be created.") );
            return false;
        }

        // ... fill the (standard) depth/stencil buffer description structure ...
        D3D11_TEXTURE2D_DESC d3d_z_buffer_desc;
        ::memset( &d3d_z_buffer_desc, 0, sizeof(d3d_z_buffer_desc) );
        d3d_z_buffer_desc.Width = back_buffer_width;
        d3d_z_buffer_desc.Height = back_buffer_height;
        d3d_z_buffer_desc.MipLevels = 1;
        d3d_z_buffer_desc.ArraySize = 1;
        d3d_z_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        d3d_z_buffer_desc.SampleDesc = dxgi_swap_chain_desc.SampleDesc;
        d3d_z_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        d3d_z_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        d3d_z_buffer_desc.CPUAccessFlags = 0;
        d3d_z_buffer_desc.MiscFlags = 0;

        // ... create the (standard) depth/stencil buffer, ...
        wrl::ComPtr<ID3D11Texture2D> d3d_z_buffer;
        hresult = _d3d_device->CreateTexture2D( &d3d_z_buffer_desc, nullptr, &d3d_z_buffer );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("ID3D11Device::CreateTexture2D") ) );
            LOG_ERROR( TEXT("The Direct3D 11 (standard) depth/stencil buffer could not be created.") );
            return false;
        }

        // ... and create the (standard) depth/stencil view interface.
        hresult = _d3d_device->CreateDepthStencilView( d3d_z_buffer.Get(), nullptr, &_d3d_depth_stencil_view );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::dxgi_result( hresult, TEXT("ID3D11Device::CreateDepthStencilView") ) );
            LOG_ERROR( TEXT("The Direct3D 11 (standard) depth/stencil view interface could not be created.") );
            return false;
        }

        // Set the (standard) render targets ...
        auto d3d_render_target_view = _d3d_render_target_view.Get();
        _d3d_immediate_context->OMSetRenderTargets( 1, &d3d_render_target_view, _d3d_depth_stencil_view.Get() );

        // ... and the (standard) viewport.
        D3D11_VIEWPORT d3d_viewport;
        ::memset( &d3d_viewport, 0, sizeof(d3d_viewport) );
        d3d_viewport.TopLeftX = 0.0f;
        d3d_viewport.TopLeftY = 0.0f;
        d3d_viewport.Width = static_cast<float>( back_buffer_width );
        d3d_viewport.Height = static_cast<float>( back_buffer_height );
        d3d_viewport.MinDepth = 0.0f;
        d3d_viewport.MaxDepth = 1.0f;
        _d3d_immediate_context->RSSetViewports( 1, &d3d_viewport );

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool RenderingDeviceImpl::_shutdown_impl()
    {
        // Release the (standard) render target view interfaces.
        _d3d_render_target_view.Reset();
        _d3d_depth_stencil_view.Reset();

        // Release the (immediate) device context interface.
        _d3d_immediate_context.Reset();

        // Release the Direct3D 11 device interface.
        _d3d_device.Reset();

        return true;
    }

    /**
     * Creates a new rendering context with the specified index (0: primary context).
     * Returns NULL if the rendering context could not be created.
     */
    RenderingContext* RenderingDeviceImpl::_create_rendering_context_impl(unsigned index)
    {
        if ( index == 0 )
            return new RenderingContextImpl( this, index, _d3d_immediate_context, _d3d_render_target_view, _d3d_depth_stencil_view );

        wrl::ComPtr<ID3D11DeviceContextN> d3d_deferred_context;
        HRESULT hresult = _d3d_device->CreateDeferredContext1( 0 , &d3d_deferred_context );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateDeferredContext") ) );
            return nullptr;
        }

        return new RenderingContextImpl( this, index, d3d_deferred_context, _d3d_render_target_view, _d3d_depth_stencil_view );
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



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Tries to find a suitable Direct3D 11 input layout based on the currently set
     * vertex buffer(s).
     */
    ID3D11InputLayout* RenderingDeviceImpl::_find_d3d_input_layout(const shared::meshes::VertexBufferBinding& vertex_buffer_binding)
    {
        auto it = _d3d_input_layouts_by_binding.find( vertex_buffer_binding );
        if ( it != _d3d_input_layouts_by_binding.end() )
            return it->second.Get();

        return nullptr;
    }

    /**
     * Tries to find a suitable Direct3D 11 input layout, and otherwise creates a
     * new one, both based on the vertex layout(s) of the currently set vertex
     * buffer(s).
     */
    ID3D11InputLayout* RenderingDeviceImpl::_find_or_create_d3d_input_layout(const shared::meshes::VertexBufferBinding& vertex_buffer_binding, const void* shader_bytecode_with_input_signature, size_t bytecode_length)
    {
        shared::meshes::ComposedVertexLayout composed_vertex_layout{ vertex_buffer_binding };

        auto it = _d3d_input_layouts_by_layout.find( composed_vertex_layout );
        if ( it != _d3d_input_layouts_by_layout.end() )
            return it->second.Get();

        std::vector<D3D11_INPUT_ELEMENT_DESC> d3d_input_element_descs;
        for ( unsigned stream_index = 0; stream_index < states::StreamStates::MAX_VERTEX_STREAMS; ++stream_index )
        {
            if ( !vertex_buffer_binding.vertex_buffer_ids[ stream_index ].is_valid() )
                continue;

            unsigned offset = 0;

            for ( const xl7::graphics::meshes::VertexLayout::Element& element : composed_vertex_layout.vertex_layouts[ stream_index ].elements )
            {
                D3D11_INPUT_ELEMENT_DESC d3d_input_element_desc;
                d3d_input_element_desc.SemanticName = _d3d_semantic_name_from( element.semantic );
                d3d_input_element_desc.SemanticIndex = element.semantic_index;
                d3d_input_element_desc.Format = _dxgi_format_from( element.data_type );
                d3d_input_element_desc.InputSlot = stream_index;
                d3d_input_element_desc.AlignedByteOffset = offset;
                d3d_input_element_desc.InputSlotClass = composed_vertex_layout.instance_data_step_rates[ stream_index ] > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
                d3d_input_element_desc.InstanceDataStepRate = composed_vertex_layout.instance_data_step_rates[ stream_index ];

                d3d_input_element_descs.push_back( d3d_input_element_desc );

                offset += static_cast<WORD>( element.get_size() );
            } // for each vertex element
        } // for each vertex stream

        auto num_elements = static_cast<unsigned>( d3d_input_element_descs.size() );

        wrl::ComPtr<ID3D11InputLayout> d3d_input_layout;
        HRESULT hresult = _d3d_device->CreateInputLayout( &d3d_input_element_descs[0], num_elements, shader_bytecode_with_input_signature, bytecode_length, &d3d_input_layout );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateInputLayout") ) );
            return nullptr;
        }

        _d3d_input_layouts_by_layout.emplace( std::make_pair( std::move(composed_vertex_layout), d3d_input_layout ) );
        _d3d_input_layouts_by_binding.insert( std::make_pair( vertex_buffer_binding, d3d_input_layout ) );

        return d3d_input_layout.Get();
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
