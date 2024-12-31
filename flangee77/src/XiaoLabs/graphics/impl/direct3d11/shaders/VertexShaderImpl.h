#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
#include "../../../shaders/VertexShader.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {
    class ResourceFactoryImpl;
namespace shaders {



class VertexShaderImpl final
    : public xl7::graphics::shaders::VertexShader
{

public:
    class Attorney
    {
        static VertexShaderImpl* create(const CreateParams<Desc>& params) { return new VertexShaderImpl(params); }
        friend class xl7::graphics::impl::direct3d11::ResourceFactoryImpl;
    };



    VertexShaderImpl() = delete;

    VertexShaderImpl(const VertexShaderImpl&) = delete;
    VertexShaderImpl& operator=(const VertexShaderImpl&) = delete;
    VertexShaderImpl(VertexShaderImpl&&) = delete;
    VertexShaderImpl& operator=(VertexShaderImpl&&) = delete;



    /**
     * Returns the Direct3D 11 vertex shader interface.
     */
    ID3D11VertexShader* get_raw_d3d_vertex_shader() const { return _d3d_vertex_shader.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    VertexShaderImpl(const CreateParams<Desc>& params);
    ~VertexShaderImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_vertex_shader.Get(); }


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
     * The Direct3D 11 vertex shader interface.
     */
    wrl::ComPtr<ID3D11VertexShader> _d3d_vertex_shader;

}; // class VertexShaderImpl



} // namespace shaders
} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
