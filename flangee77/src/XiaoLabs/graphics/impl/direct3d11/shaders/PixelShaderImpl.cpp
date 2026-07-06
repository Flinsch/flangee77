#include "PixelShaderImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include "../../shared/shaders/D3DShaderCompiler.h"

#include "../../../shaders/FileIncludeHandler.h"

#include "./D3DShaderReflection.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>



namespace xl7::graphics::impl::direct3d11::shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    PixelShaderImpl::PixelShaderImpl(const CreateContext& ctx, const graphics::shaders::ShaderDesc& desc)
        : PixelShader(ctx, desc)
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool PixelShaderImpl::_acquire_impl()
    {
        auto* d3d_device = GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->get_raw_d3d_device();
        assert(d3d_device);

        HRESULT hresult = d3d_device->CreatePixelShader(
            get_bytecode().data(),
            get_bytecode().size(),
            nullptr,
            &_d3d_pixel_shader);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d11_result(hresult, u8"ID3D11Device::CreatePixelShader"));
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool PixelShaderImpl::_dispose_impl()
    {
        GraphicsSystem::instance().get_rendering_device_impl<RenderingDeviceImpl>()->_release_d3d_constant_buffers(get_id());

        _d3d_pixel_shader.Reset();

        return true;
    }



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
     * (Re)compiles the given high-level shader code and returns the compiled
     * bytecode (or empty data on failure).
     */
    cl7::byte_vector PixelShaderImpl::_compile_impl(cl7::byte_view code_data, const graphics::shaders::CompileOptions& compile_options)
    {
        const cl7::Version& version = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.max_pixel_shader_profile;
        const cl7::astring entry_point = _cascade_entry_point(compile_options);

        graphics::shaders::FileIncludeHandler include_handler;
        shared::shaders::D3DShaderCompiler shader_compiler(get_type(), version, &include_handler);

        return shader_compiler.compile_source_code(cl7::text::codec::to_utf8_unchecked(code_data), u8"", compile_options, entry_point);
    }

    /**
     * Performs a "reflection" on the given (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    graphics::shaders::ReflectionResult PixelShaderImpl::_reflect_impl(cl7::byte_view bytecode)
    {
        return D3DShaderReflection::reflect(bytecode);
    }

    /**
     * Recreates the shader resource after recompiling the high-level code, or
     * whatever is necessary to effectively incorporate the newly compiled bytecode.
     */
    bool PixelShaderImpl::_on_recompile_impl(cl7::byte_view bytecode)
    {
        return _acquire_impl();
    }



} // namespace xl7::graphics::impl::direct3d11::shaders
