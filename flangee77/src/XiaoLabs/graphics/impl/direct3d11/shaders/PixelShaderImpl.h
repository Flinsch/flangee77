#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"



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
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Releases/"unacquires" the resource.
     */
    virtual bool _release_impl() override;



    // #############################################################################
    // Shader Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

}; // class PixelShaderImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
