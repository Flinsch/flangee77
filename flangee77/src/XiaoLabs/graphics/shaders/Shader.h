#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../resources/Resource.h"

#include "./ShaderCode.h"
#include "./CodeDataProvider.h"
#include "./MacroDefinitions.h"
#include "./ConstantBufferDeclaration.h"
#include "./TextureSamplerDeclaration.h"

#include <vector>



namespace xl7 {
namespace graphics {
namespace shaders {



class ShaderManager;



class Shader
    : public resources::Resource
{

public:
    enum class Type
    {
        VertexShader,
        PixelShader,
    };

public:
    struct Desc
    {
        /** The language of the (source) code on which the shader is based. */
        ShaderCode::Language language;
        /** The name of the shader entry point (can be empty, especially for precompiled shaders). */
        cl7::astring entry_point;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Shader(Type type, const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Shader() = default;

private:
    /** Default constructor. */
    Shader() = delete;
    /** Copy constructor. */
    Shader(const Shader&) = delete;
    /** Copy assignment operator. */
    Shader& operator = (const Shader&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The type of the shader.
     */
    const Type _type;

    /**
     * The descriptor of the shader.
     */
    const Desc _desc;

private:
    /**
     * The compiled bytecode.
     */
    ShaderCode _bytecode;

    /**
     * The constant buffer declarations.
     */
    std::vector<ConstantBufferDeclaration> _constant_buffer_declarations;

    /**
     * The texture/sampler declarations.
     */
    std::vector<TextureSamplerDeclaration> _texture_sampler_declarations;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the type of the shader.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the shader.
     */
    const Desc& get_desc() const { return _desc; }

public:
    /**
     * Indicates whether the shader is precompiled (based on bytecode).
     */
    bool is_precompiled() const { return _desc.language == ShaderCode::Language::Bytecode; }

    /**
     * Indicates whether the shader is precompiled (based on high-level code).
     */
    bool is_recompilable() const { return _desc.language == ShaderCode::Language::HighLevel; }

public:
    /**
     * Returns the compiled bytecode.
     */
    const ShaderCode& get_bytecode() const { return _bytecode; }

    /**
     * Returns the constant buffer declarations.
     */
    const std::vector<ConstantBufferDeclaration>& get_constant_buffer_declarations() const { return _constant_buffer_declarations; }

    /**
     * Returns the texture/sampler declarations.
     */
    const std::vector<TextureSamplerDeclaration>& get_texture_sampler_declarations() const { return _texture_sampler_declarations; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    bool recompile(const MacroDefinitions& macro_definitions);



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& data_provider) override;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("shader"); }



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Requests/acquires a precompiled shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_precompiled_impl(const CodeDataProvider& code_data_provider, std::vector<ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<TextureSamplerDeclaration>& texture_sampler_declarations_out) = 0;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const CodeDataProvider& code_data_provider, ShaderCode& bytecode_out, std::vector<ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<TextureSamplerDeclaration>& texture_sampler_declarations_out) = 0;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const MacroDefinitions& macro_definitions, ShaderCode& bytecode_out, std::vector<ConstantBufferDeclaration>& constant_buffer_declarations_out, std::vector<TextureSamplerDeclaration>& texture_sampler_declarations_out) = 0;

}; // class Shader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_SHADER_H
