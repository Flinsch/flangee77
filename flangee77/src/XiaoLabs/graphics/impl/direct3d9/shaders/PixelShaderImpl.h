#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
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
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

    /**
     * The Direct3D 9 pixel shader interface.
     */
    wrl::ComPtr<IDirect3DPixelShader9> _d3d_pixel_shader;



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _release_impl() override;



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
    virtual bool _acquire_precompiled_impl(const xl7::graphics::shaders::CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const xl7::graphics::shaders::CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

}; // class PixelShaderImpl



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
