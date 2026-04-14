#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {
    class ResourceFactoryImpl;
namespace shaders {



class PixelShaderImpl final
    : public graphics::shaders::PixelShader
{

public:
    class Attorney
    {
        static PixelShaderImpl* create(const CreateContext& ctx, const graphics::shaders::ShaderDesc& desc) { return new PixelShaderImpl(ctx, desc); }
        friend class direct3d11::ResourceFactoryImpl;
    };



    PixelShaderImpl() = delete;

    PixelShaderImpl(const PixelShaderImpl&) = delete;
    PixelShaderImpl& operator=(const PixelShaderImpl&) = delete;
    PixelShaderImpl(PixelShaderImpl&&) = delete;
    PixelShaderImpl& operator=(PixelShaderImpl&&) = delete;



    /**
     * Returns the Direct3D 11 pixel shader interface.
     */
    ID3D11PixelShader* get_raw_d3d_pixel_shader() const { return _d3d_pixel_shader.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    explicit PixelShaderImpl(const CreateContext& ctx, const graphics::shaders::ShaderDesc& desc);
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
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool _acquire_impl() override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
     * (Re)compiles the given high-level shader code and returns the compiled
     * bytecode (or empty data on failure).
     */
    cl7::byte_vector _compile_impl(cl7::byte_view code_data, const graphics::shaders::CompileOptions& compile_options) override;

    /**
     * Performs a "reflection" on the given (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    graphics::shaders::ReflectionResult _reflect_impl(cl7::byte_view bytecode) override;

    /**
     * Recreates the shader resource after recompiling the high-level code, or
     * whatever is necessary to effectively incorporate the newly compiled bytecode.
     */
    bool _on_recompile_impl(cl7::byte_view bytecode) override;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 11 pixel shader interface.
     */
    wrl::ComPtr<ID3D11PixelShader> _d3d_pixel_shader;

}; // class PixelShaderImpl



} // namespace shaders
} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_PIXELSHADERIMPL_H
