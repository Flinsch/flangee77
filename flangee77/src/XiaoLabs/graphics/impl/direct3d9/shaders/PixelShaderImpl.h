#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace shaders {



class PixelShaderImpl final
    : public xl7::graphics::shaders::PixelShader
{

public:
    class Attorney
    {
        static PixelShaderImpl* create(const CreateParams<Desc>& params) { return new PixelShaderImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    PixelShaderImpl() = delete;

    PixelShaderImpl(const PixelShaderImpl&) = delete;
    PixelShaderImpl& operator = (const PixelShaderImpl&) = delete;
    PixelShaderImpl(PixelShaderImpl&&) = delete;
    PixelShaderImpl& operator = (PixelShaderImpl&&) = delete;



    /**
     * Returns the Direct3D 9 pixel shader interface.
     */
    IDirect3DPixelShader9* get_raw_d3d_pixel_shader() const { return _d3d_pixel_shader.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    PixelShaderImpl(const CreateParams<Desc>& params);
    ~PixelShaderImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_pixel_shader.Get(); }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
     * Requests/acquires a precompiled shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    bool _acquire_precompiled_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider) override;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    bool _acquire_recompilable_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider, xl7::graphics::shaders::ShaderCode& bytecode_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    bool _recompile_impl(const xl7::graphics::shaders::CompileOptions& compile_options, xl7::graphics::shaders::ShaderCode& bytecode_out) override;

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    bool _reflect_impl(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out) override;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 9 pixel shader interface.
     */
    wrl::ComPtr<IDirect3DPixelShader9> _d3d_pixel_shader;

}; // class PixelShaderImpl



} // namespace shaders
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
