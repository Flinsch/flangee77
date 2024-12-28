#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../resources/Resource.h"

#include "./ShaderCode.h"
#include "./CodeDataProvider.h"
#include "./CompileOptions.h"
#include "./ReflectionResult.h"
#include "./ConstantBufferMapping.h"

#include <unordered_map>



namespace xl7::graphics::shaders {



class ShaderManager;

class ConstantBuffer;



class Shader // NOLINT(*-virtual-class-destructor)
    : public resources::Resource
{

public:
    enum class Type
    {
        VertexShader,
        PixelShader,
    };

    struct Desc
    {
        /** The language of the (source) code on which the shader is based. */
        ShaderCode::Language language;
        /** The name of the shader entry point (can be empty, especially for precompiled shaders). */
        cl7::u8string entry_point;
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
    const Desc& get_desc() const { return _desc; }

    /**
     * Indicates whether the shader is precompiled (based on bytecode).
     */
    bool is_precompiled() const { return _desc.language == ShaderCode::Language::Bytecode; }

    /**
     * Indicates whether the shader is precompiled (based on high-level code).
     */
    bool is_recompilable() const { return _desc.language == ShaderCode::Language::HighLevel; }

    /**
     * Returns the compiled bytecode.
     */
    const ShaderCode& get_bytecode() const { return _bytecode; }

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
    const ConstantBufferDeclaration* find_constant_buffer_declaration(cl7::u8string_view constant_buffer_name) const;

    /**
     * Searches for the specified constant declaration and returns it along with the
     * associated constant buffer declaration if found, NULL twice otherwise.
     */
    std::pair<const ConstantBufferDeclaration*, const ConstantDeclaration*> find_constant_buffer_and_constant_declaration(cl7::u8string_view constant_name) const;

    /**
     * Searches for the specified texture/sampler declaration and returns it if
     * found, NULL otherwise.
     */
    const TextureSamplerDeclaration* find_texture_sampler_declaration(cl7::u8string_view texture_sampler_name) const;



protected:
    Shader(Type type, const CreateParams<Desc>& params);
    ~Shader() override = default;



    /**
     * Returns the effective name of the entry point for (re)compiling the shader.
     */
    cl7::u8string _cascade_entry_point(const CompileOptions& compile_options) const;

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc. and validates the result.
     */
    bool _reflect_and_validate(const ShaderCode& bytecode, ReflectionResult& reflection_result_out);

    /**
     * Validates the given reflection result.
     */
    static bool _validate_reflection_result(const ReflectionResult& reflection_result);



private:

    /**
     * Requests/acquires a precompiled shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_precompiled_impl(const CodeDataProvider& code_data_provider) = 0;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const CodeDataProvider& code_data_provider, ShaderCode& bytecode_out) = 0;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const CompileOptions& compile_options, ShaderCode& bytecode_out) = 0;

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    virtual bool _reflect_impl(const ShaderCode& bytecode, ReflectionResult& reflection_result_out) = 0;



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const resources::DataProvider& data_provider) override;



    /**
     * The type of the shader.
     */
    const Type _type;

    /**
     * The descriptor of the shader.
     */
    const Desc _desc;

    /**
     * The compiled bytecode.
     */
    ShaderCode _bytecode;

    /**
     * The reflection result, which includes parameter declarations etc.
     */
    ReflectionResult _reflection_result;

    /**
     * The constant buffer mappings cached so far by constant buffer ID.
     */
    mutable std::unordered_map<resources::ResourceID, ConstantBufferMapping> _constant_buffer_mappings_by_constant_buffer_id;

}; // class Shader



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADER_H
