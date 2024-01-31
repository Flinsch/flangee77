#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
#include "../../../shaders/VertexShader.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace shaders {



class VertexShaderImpl final
    : public xl7::graphics::shaders::VertexShader
{

public:
    class Attorney
    {
        static VertexShaderImpl* create(const CreateParams<Desc>& params) { return new VertexShaderImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexShaderImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~VertexShaderImpl() = default;

private:
    /** Default constructor. */
    VertexShaderImpl() = delete;
    /** Copy constructor. */
    VertexShaderImpl(const VertexShaderImpl&) = delete;
    /** Copy assignment operator. */
    VertexShaderImpl& operator = (const VertexShaderImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

    /**
     * The Direct3D 9 vertex shader interface.
     */
    wrl::ComPtr<IDirect3DVertexShader9> _d3d_vertex_shader;



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
    virtual bool _acquire_precompiled_impl(const CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ParameterTable& parameter_table_out) override;

}; // class VertexShaderImpl



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
