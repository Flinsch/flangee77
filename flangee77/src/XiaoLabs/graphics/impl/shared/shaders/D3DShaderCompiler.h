#ifndef XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
#define XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
#include "../../../shaders/ShaderCompiler.h"



namespace xl7::graphics::impl::shared::shaders {



class D3DShaderCompiler
    : public graphics::shaders::ShaderCompiler
{

public:
    using ShaderCompiler::ShaderCompiler;

    ~D3DShaderCompiler() override = default;



private:
    /**
     * Compiles the given high-level code into bytecode. The specified include path
     * is used to resolve any (local) #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector _compile_source_code(const cl7::u8string& hlsl_code, const cl7::u8string& include_path, const graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point) override;

}; // class D3DShaderCompiler



} // namespace xl7::graphics::impl::shared::shaders

#endif // XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
