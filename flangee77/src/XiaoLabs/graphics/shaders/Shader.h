#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADER_H
#define XL7_GRAPHICS_SHADERS_SHADER_H
#include "../../Resource.h"

#include "./ShaderCode.h"
#include "./MacroDefinitions.h"
#include "./ParameterTable.h"



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
    // Prototypes
    // #############################################################################
private:
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
