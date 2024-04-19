#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class PixelShaderImpl final
    : public xl7::graphics::shaders::PixelShader
{

public:
    class Attorney
    {
        static PixelShaderImpl* create(const CreateParams<Desc>& params) { return new PixelShaderImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    PixelShaderImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~PixelShaderImpl() = default;

private:
    /** Default constructor. */
    PixelShaderImpl() = delete;
    /** Copy constructor. */
    PixelShaderImpl(const PixelShaderImpl&) = delete;
    /** Copy assignment operator. */
    PixelShaderImpl& operator = (const PixelShaderImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 pixel shader interface.
     */
    wrl::ComPtr<ID3D11PixelShader> _d3d_pixel_shader;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 pixel shader interface.
     */
    ID3D11PixelShader* get_raw_d3d_pixel_shader() const { return _d3d_pixel_shader.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return _d3d_pixel_shader.Get(); }

private:
    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _dispose_impl() override;



    // #############################################################################
    // Shader Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires a precompiled shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_precompiled_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider, std::vector<xl7::graphics::shaders::ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<xl7::graphics::shaders::TextureSamplerDeclaration>& texture_sampler_declarations_out) override;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider, xl7::graphics::shaders::ShaderCode& bytecode_out, std::vector<xl7::graphics::shaders::ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<xl7::graphics::shaders::TextureSamplerDeclaration>& texture_sampler_declarations_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ShaderCode& bytecode_out, std::vector<xl7::graphics::shaders::ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<xl7::graphics::shaders::TextureSamplerDeclaration>& texture_sampler_declarations_out) override;

}; // class PixelShaderImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
