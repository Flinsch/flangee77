#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
#include "../../../shaders/VertexShader.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
    class ResourceFactoryImpl;
namespace shaders {



class VertexShaderImpl final
    : public xl7::graphics::shaders::VertexShader
{

public:
    class Attorney
    {
        static VertexShaderImpl* create(const CreateParams<Desc>& params) { return new VertexShaderImpl( params ); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
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
     * The Direct3D 9 vertex shader interface.
     */
    wrl::ComPtr<IDirect3DVertexShader9> _d3d_vertex_shader;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 vertex shader interface.
     */
    IDirect3DVertexShader9* get_raw_d3d_vertex_shader() const { return _d3d_vertex_shader.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return _d3d_vertex_shader.Get(); }

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
    virtual bool _acquire_precompiled_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider) override;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider, xl7::graphics::shaders::ShaderCode& bytecode_out) override;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const xl7::graphics::shaders::CompileOptions& compile_options, xl7::graphics::shaders::ShaderCode& bytecode_out) override;

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    virtual bool _reflect_impl(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out) override;

}; // class VertexShaderImpl



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_VERTEXSHADERIMPL_H
