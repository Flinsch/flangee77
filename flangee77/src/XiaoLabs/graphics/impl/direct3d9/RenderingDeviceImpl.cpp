#include "RenderingDeviceImpl.h"

#include "./GraphicsSystemImpl.h"
#include "./RenderingContextImpl.h"
#include "./ResourceFactoryImpl.h"
#include "./mappings.h"
#include "./errors.h"

#include <XiaoLabs/MainWindow.h>
#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>

#include <dxgi.h>



namespace xl7::graphics::impl::direct3d9 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingDeviceImpl::RenderingDeviceImpl()
        : RenderingDevice(std::unique_ptr<IResourceFactory>(ResourceFactoryImpl::Attorney::create()))
    {
        ::memset(&_d3d_adapter_identifier, 0, sizeof(_d3d_adapter_identifier));
        ::memset(&_d3d_present_parameters, 0, sizeof(_d3d_present_parameters));
        ::memset(&_d3d_device_caps, 0, sizeof(_d3d_device_caps));
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
        auto* const d3d_main = static_cast<GraphicsSystemImpl*>(&GraphicsSystem::instance())->get_raw_d3d_main(); // NOLINT(*-pro-type-static-cast-downcast)

        // Cache the main window's display mode.
        const MainWindow::DisplayMode window_display_mode = MainWindow::instance().get_display_mode();
        const bool fullscreen = window_display_mode == MainWindow::DisplayMode::Fullscreen;

        // Set the format of the back buffer
        // and the depth/stencil surface.
        constexpr D3DFORMAT back_buffer_format = D3DFMT_X8R8G8B8;
        constexpr D3DFORMAT depth_stencil_format = D3DFMT_D24S8;

        // Fill the presentation parameters structure.
        ::memset(&_d3d_present_parameters, 0, sizeof(_d3d_present_parameters));
        _d3d_present_parameters.BackBufferWidth             = get_back_buffer_width();
        _d3d_present_parameters.BackBufferHeight            = get_back_buffer_height();
        _d3d_present_parameters.BackBufferFormat            = back_buffer_format;
        _d3d_present_parameters.BackBufferCount             = GraphicsSystem::instance().get_config().video.back_buffer_count;
        _d3d_present_parameters.MultiSampleType             = D3DMULTISAMPLE_NONE;
        _d3d_present_parameters.MultiSampleQuality          = 0;
        _d3d_present_parameters.SwapEffect                  = D3DSWAPEFFECT_DISCARD;
        _d3d_present_parameters.hDeviceWindow               = MainWindow::instance().get_handle();
        _d3d_present_parameters.Windowed                    = fullscreen ? FALSE : TRUE;
        _d3d_present_parameters.EnableAutoDepthStencil      = TRUE;
        _d3d_present_parameters.AutoDepthStencilFormat      = depth_stencil_format;
        _d3d_present_parameters.Flags                       = depth_stencil_format != D3DFMT_D16_LOCKABLE ? D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL : 0;
        _d3d_present_parameters.FullScreen_RefreshRateInHz  = fullscreen ? GraphicsSystem::instance().get_config().video.refresh_rate : D3DPRESENT_RATE_DEFAULT;
        _d3d_present_parameters.PresentationInterval        = GraphicsSystem::instance().get_config().video.vsync_enabled ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

        // (Try to) create the hardware-based
        // Direct3D 9 device interface.
        HRESULT hresult = d3d_main->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            MainWindow::instance().get_handle(),
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &_d3d_present_parameters,
            &_d3d_device);

        if (hresult != D3D_OK)
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3D9::CreateDevice"));
            LOG_ERROR(u8"The Direct3D 9 device interface could not be created.");
            return false;
        }

        // Ask for the structure with information
        // to identify the adapter.
        hresult = d3d_main->GetAdapterIdentifier(
            D3DADAPTER_DEFAULT,
            0,
            &_d3d_adapter_identifier);
        if (FAILED(hresult))
            LOG_WARNING(errors::d3d9_result(hresult, u8"IDirect3D9::GetAdapterIdentifier"));

        // Capture the device capabilities.
        hresult = _d3d_device->GetDeviceCaps(&_d3d_device_caps);
        if (FAILED(hresult))
            LOG_WARNING(errors::d3d9_result(hresult, u8"IDirect3DDevice9::GetDeviceCaps"));

        // Adopt the supported shader versions.
        capabilities.shaders.vertex_shader_version = {
            .major = static_cast<unsigned>(D3DSHADER_VERSION_MAJOR(_d3d_device_caps.VertexShaderVersion)),
            .minor = static_cast<unsigned>(D3DSHADER_VERSION_MINOR(_d3d_device_caps.VertexShaderVersion)),
        };
        capabilities.shaders.pixel_shader_version = {
            .major = static_cast<unsigned>(D3DSHADER_VERSION_MAJOR(_d3d_device_caps.PixelShaderVersion)),
            .minor = static_cast<unsigned>(D3DSHADER_VERSION_MINOR(_d3d_device_caps.PixelShaderVersion)),
        };

        // Adopt the supported texture capabilities.
        capabilities.textures.max_texture_2d_width = _d3d_device_caps.MaxTextureWidth;
        capabilities.textures.max_texture_2d_height = _d3d_device_caps.MaxTextureHeight;
        capabilities.textures.max_texture_3d_size = _d3d_device_caps.MaxVolumeExtent;
        capabilities.textures.max_cubemap_size = (std::min)(_d3d_device_caps.MaxTextureWidth, _d3d_device_caps.MaxTextureHeight);
        capabilities.textures.max_texture_array_size = 0; // Not supported at all.
        capabilities.textures.texture_2d_pow2_only = (_d3d_device_caps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0; // Ignore D3DPTEXTURECAPS_NONPOW2CONDITIONAL?
        capabilities.textures.texture_3d_pow2_only = (_d3d_device_caps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2) != 0;
        capabilities.textures.cubemap_pow2_only = (_d3d_device_caps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2) != 0;
        capabilities.textures.square_only = (_d3d_device_caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0;
        capabilities.textures.max_aspect_ratio = _d3d_device_caps.MaxTextureAspectRatio;

        // Adopt other capabilities.
        capabilities.max_simultaneous_render_target_count = _d3d_device_caps.NumSimultaneousRTs;
        capabilities.max_concurrent_vertex_stream_count = _d3d_device_caps.MaxStreams;
        capabilities.max_constant_buffer_slot_count = pipeline::AbstractShaderStage::MAX_CONSTANT_BUFFER_SLOTS; // There are no actual constant buffers in Direct3D 9. Just take the framework limit.
        capabilities.max_texture_sampler_slot_count = _d3d_device_caps.MaxSimultaneousTextures;

        // And more.
        capabilities.max_anisotropy = _d3d_device_caps.MaxAnisotropy;

        // (Try to) determine the available
        // video memory composition.
        _determine_video_memory(capabilities.memory);

        // Grab the (standard) render target surface interface ...
        hresult = _d3d_device->GetRenderTarget(0, &_d3d_render_target_surface);
        if (hresult != D3D_OK)
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3D9::GetRenderTarget"));
            LOG_ERROR(u8"The Direct3D 9 (standard) render target surface interface could not be queried.");
            return false;
        }

        // ... and the (standard) depth/stencil surface interface.
        hresult = _d3d_device->GetDepthStencilSurface(&_d3d_depth_stencil_surface);
        if (hresult != D3D_OK)
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3D9::GetDepthStencilSurface"));
            LOG_ERROR(u8"The Direct3D 9 (standard) depth/stencil surface interface could not be queried.");
            return false;
        }

        // To be on the safe side, set the (standard) viewport.
        D3DVIEWPORT9 d3d_viewport = {};
        d3d_viewport.X = 0;
        d3d_viewport.Y = 0;
        d3d_viewport.Width = get_back_buffer_width();
        d3d_viewport.Height = get_back_buffer_height();
        d3d_viewport.MinZ = 0.0f;
        d3d_viewport.MaxZ = 1.0f;
        hresult = _d3d_device->SetViewport(&d3d_viewport);
        if (hresult != D3D_OK)
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3D9::SetViewport"));
            LOG_ERROR(u8"The Direct3D 9 (standard) viewport could not be set.");
            return false;
        }

        return true;
    }

    /**
     * De-initializes the component.
     */
    bool RenderingDeviceImpl::_shutdown_impl()
    {
        // Release the (standard) render target surface interfaces.
        _d3d_render_target_surface.Reset();
        _d3d_depth_stencil_surface.Reset();

        // Release the Direct3D 9 device interface.
        _d3d_device.Reset();

        return true;
    }

    /**
     * Creates a new rendering context with the specified index (0: primary context).
     * Returns NULL if the rendering context could not be created.
     */
    RenderingContext* RenderingDeviceImpl::_create_rendering_context_impl(unsigned index)
    {
        assert(index == 0);
        if (index == 0)
            return new RenderingContextImpl(this, index, _d3d_device, _d3d_render_target_surface, _d3d_depth_stencil_surface);

        LOG_ERROR(u8"Direct3D 9 does not support multiple rendering contexts.");
        return nullptr;
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
        HRESULT hresult = _d3d_device->Present(nullptr, nullptr, nullptr, nullptr);

        if (hresult == D3DERR_DEVICEREMOVED)
        {
            _notify_device_lost();
            return false;
        }

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::Present"));
            return false;
        }

        return true;
    }

    /**
     * Checks whether the device (generally) supports the specified combination of
     * pixel format and channel order for the specified texture type.
     */
    bool RenderingDeviceImpl::_check_texture_format_impl(textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder channel_order)
    {
        D3DFORMAT d3d_adapter_format = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_d3d_present_parameters().BackBufferFormat;
        D3DFORMAT d3d_texture_format = mappings::_d3d_format_from(pixel_format, channel_order);

        D3DRESOURCETYPE d3d_rtype;
        switch (texture_type)
        {
        case textures::Texture::Type::Texture2D:        d3d_rtype = D3DRTYPE_TEXTURE;       break;
        case textures::Texture::Type::Texture3D:        d3d_rtype = D3DRTYPE_VOLUMETEXTURE; break;
        case textures::Texture::Type::Cubemap:          d3d_rtype = D3DRTYPE_CUBETEXTURE;   break;
        default:
            assert(texture_type == textures::Texture::Type::Texture2DArray);
            LOG_ERROR(u8"Direct3D 9 does not support 2D texture arrays.");
            return false;
        }

        HRESULT hresult = static_cast<GraphicsSystemImpl&>(GraphicsSystem::instance()).get_raw_d3d_main()->CheckDeviceFormat( // NOLINT(*-pro-type-static-cast-downcast)
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            d3d_adapter_format,
            0,
            d3d_rtype,
            d3d_texture_format);

        if (hresult == D3DERR_NOTAVAILABLE)
            return false;

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3D9::CheckDeviceFormat"));
            return false;
        }

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Tries to determine the available video memory composition.
     */
    bool RenderingDeviceImpl::_determine_video_memory(Capabilities::Memory& memory_capabilities)
    {
        ::memset(&memory_capabilities, 0, sizeof(memory_capabilities));

        const HMODULE hDXGI = ::LoadLibraryW(L"dxgi.dll");
        if (!hDXGI)
        {
            LOG_WARNING(cl7::errors::system_result(::GetLastError(), u8"::LoadLibrary"));
            return false;
        }

        using CREATEDXGIFACTORYPROC = std::add_pointer_t<HRESULT (REFIID, void**)>;
        auto CreateDXGIFactoryProc = reinterpret_cast<CREATEDXGIFACTORYPROC>(::GetProcAddress(hDXGI, "CreateDXGIFactory1"));
        if (!CreateDXGIFactoryProc)
        {
            LOG_WARNING(cl7::errors::system_result(::GetLastError(), u8"::GetProcAddress"));
            ::FreeLibrary(hDXGI);
            return false;
        }

        IDXGIFactory* factory = nullptr;
        HRESULT hresult = CreateDXGIFactoryProc(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
        if (FAILED(hresult))
        {
            LOG_WARNING(cl7::errors::system_result(hresult, u8"::CreateDXGIFactory1"));
            ::FreeLibrary(hDXGI);
            return false;
        }

        assert(factory);

        const cl7::u8string adapter_name{cl7::strings::reinterpret_utf8(_d3d_adapter_identifier.DeviceName)};
        assert(!adapter_name.empty());

        for (unsigned i = 0; ; ++i)
        {
            IDXGIAdapter* adapter = nullptr;
            hresult = factory->EnumAdapters(i, &adapter);
            if (FAILED(hresult))
                break;

            assert(adapter);

            DXGI_ADAPTER_DESC adapter_desc = {};
            hresult = adapter->GetDesc(&adapter_desc);
            adapter->Release();
            if (FAILED(hresult))
            {
                LOG_WARNING(cl7::errors::system_result(hresult, u8"IDXGIAdapter::GetDesc"));
                continue;
            }

            if (memory_capabilities.dedicated_video_memory > 0 && cl7::strings::to_utf8(cl7::strings::reinterpret_utf16(adapter_desc.Description)) != adapter_name)
                continue;

            memory_capabilities.dedicated_video_memory = adapter_desc.DedicatedVideoMemory;
            memory_capabilities.dedicated_system_memory = adapter_desc.DedicatedSystemMemory;
            memory_capabilities.shared_system_memory = adapter_desc.SharedSystemMemory;
        } // for each adapter

        factory->Release();

        ::FreeLibrary(hDXGI);

        return true;
    }



} // namespace xl7::graphics::impl::direct3d9
