#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../Resource.h"

#include "./ShaderCode.h"
#include "./MacroDefinitions.h"
#include "./ParameterTable.h"

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace shaders {



class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;



class ShaderManager;



class Shader
    : public Resource
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

    struct CodeProvider
        : public DefaultDataProvider
    {
        const ShaderCode& shader_code;
        const MacroDefinitions& macro_definitions;

        CodeProvider(const ShaderCode& shader_code) : CodeProvider( shader_code, {} ) {}
        CodeProvider(const ShaderCode& shader_code, const MacroDefinitions& macro_definitions) : DefaultDataProvider( shader_code.get_code_data() ), shader_code( shader_code ), macro_definitions( macro_definitions ) {}
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
     * The table of constant/sampler parameters.
     */
    ParameterTable _parameter_table;



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
     * Returns the table of constant/sampler parameters.
     */
    const ParameterTable& get_parameter_table() const { return _parameter_table; }



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
    virtual bool _check_impl(const DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const DataProvider& data_provider) override;

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
    virtual bool _acquire_precompiled_impl(const CodeProvider& code_provider, ParameterTable& parameter_table_out) = 0;

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_recompilable_impl(const CodeProvider& code_provider, ParameterTable& parameter_table_out) = 0;

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    virtual bool _recompile_impl(const MacroDefinitions& macro_definitions, ParameterTable& parameter_table_out) = 0;

}; // class Shader



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_SHADER_H
