#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../resources/ResourceWithData.h"

#include "./ShaderDesc.h"
#include "./ShaderWrite.h"
#include "./CompileOptions.h"
#include "./ReflectionResult.h"
#include "./ConstantBufferMapping.h"

#include <unordered_map>



namespace xl7::graphics::shaders {



class ShaderManager;

class ConstantBuffer;



class Shader
    : public resources::ResourceWithData<Shader, resources::Resource, true>
{

public:
    class Attorney
    {
        static void build(Shader* shader, const ShaderCode& shader_code, const CompileOptions& compile_options) { shader->_build(shader_code, compile_options); }
        friend class ShaderManager;
    };



    enum struct Type
    {
        VertexShader,
        PixelShader,
    };



    Shader() = delete;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"shader"; }

    /**
     * Returns the type of the shader.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the shader.
     */
    const ShaderDesc& get_desc() const { return _desc; }

    /**
     * Indicates whether the shader is precompiled (based on bytecode).
     */
    bool is_precompiled() const { return _desc.language == ShaderCode::Language::Bytecode; }

    /**
     * Indicates whether the shader is recompilable (based on high-level code).
     */
    bool is_recompilable() const { return _desc.language == ShaderCode::Language::HighLevel; }

    /**
     * Returns the compiled bytecode.
     */
    cl7::byte_view get_bytecode() const { return _bytecode; }

    /**
     * Returns the reflection result, which includes parameter declarations etc.
     */
    const ReflectionResult& get_reflection_result() const { return _reflection_result; }



    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    bool recompile(const CompileOptions& compile_options);

    /**
     * Finds or creates the constant buffer mapping for the specified constant
     * buffer based on the reflection result of this shader. Returns NULL if not
     * applicable.
     * The padded sizes of the constant declarations specified in the descriptor of
     * the constant buffer should be set correctly to minimize the resulting mapping
     * entries as much as possible.
     */
    const ConstantBufferMapping* find_or_create_constant_buffer_mapping(const ConstantBuffer* constant_buffer) const;

    /**
     * Searches for the specified constant buffer declaration and returns it if
     * found, NULL otherwise.
     */
    const ConstantBufferDeclaration* find_constant_buffer_declaration(cl7::astring_view constant_buffer_name) const;

    /**
     * Searches for the specified constant declaration and returns it along with the
     * associated constant buffer declaration if found, NULL twice otherwise.
     */
    std::pair<const ConstantBufferDeclaration*, const ConstantDeclaration*> find_constant_buffer_and_constant_declaration(cl7::astring_view constant_name) const;

    /**
     * Searches for the specified texture/sampler declaration and returns it if
     * found, NULL otherwise.
     */
    const TextureSamplerDeclaration* find_texture_sampler_declaration(cl7::astring_view texture_sampler_name) const;



protected:

    Shader(const CreateContext& ctx, Type type, ShaderDesc desc);

    ~Shader() override = default;



    /**
     * Returns the effective name of the entry point for (re)compiling the shader.
     */
    cl7::astring _cascade_entry_point(const CompileOptions& compile_options) const;

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc. and validates the result.
     */
    ReflectionResult _reflect_and_validate(cl7::byte_view bytecode);

    /**
     * Validates the given reflection result.
     */
    static bool _validate_reflection_result(const ReflectionResult& reflection_result);



private:

    /**
     * (Re)compiles the given high-level shader code and returns the compiled
     * bytecode (or empty data on failure).
     */
    virtual cl7::byte_vector _compile_impl(cl7::byte_view code_data, const CompileOptions& compile_options) = 0;

    /**
     * Performs a "reflection" on the given (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    virtual ReflectionResult _reflect_impl(cl7::byte_view bytecode) = 0;

    /**
     * Recreates the shader resource after recompiling the high-level code, or
     * whatever is necessary to effectively incorporate the newly compiled bytecode.
     */
    virtual bool _on_recompile_impl(cl7::byte_view bytecode) = 0;



    /**
     * Takes the given shader code and compiles it if necessary, so that bytecode is
     * available for further processing.
     */
    void _build(const ShaderCode& shader_code, const CompileOptions& compile_options);

    /**
     * Compiles the high-level shader code (taken from the shader's data) and
     * returns the resulting bytecode.
     */
    cl7::byte_vector _compile(const CompileOptions& compile_options);



    /**
     * The type of the shader.
     */
    const Type _type;

    /**
     * The descriptor of the shader.
     */
    const ShaderDesc _desc;

    /**
     * The compiled bytecode.
     */
    cl7::byte_vector _bytecode;

    /**
     * The reflection result, which includes parameter declarations etc.
     */
    ReflectionResult _reflection_result;

    /**
     * The constant buffer mappings cached so far by constant buffer ID.
     */
    mutable std::unordered_map<resources::ResourceId, ConstantBufferMapping> _constant_buffer_mappings_by_constant_buffer_id;

}; // class Shader



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADER_H
