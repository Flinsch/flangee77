#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CODEPROVIDER_H
#define XL7_GRAPHICS_SHADERS_CODEPROVIDER_H
#include "../../resources/DefaultDataProvider.h"

#include "./ShaderCode.h"
#include "./MacroDefinitions.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class CodeProvider
    : public resources::DefaultDataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    CodeProvider(const ShaderCode* shader_code, const MacroDefinitions* macro_definitions);

    /**
     * Destructor.
     */
    virtual ~CodeProvider() = default;



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /** A reference to the shader code. */
    const ShaderCode& _shader_code;
    /** A reference to the macro definitions. */
    const MacroDefinitions& _macro_definitions;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /** Returns a reference to the shader code. */
    const ShaderCode& get_shader_code() const { return _shader_code; }
    /** Returns a reference to the macro definitions. */
    const MacroDefinitions& get_macro_definitions() const { return _macro_definitions; }

    /** Returns the language in which the code is given. */
    ShaderCode::Language get_language() const { return _shader_code.get_language(); }
    /** Returns the actual code data. */
    const cl7::byte_vector& get_code_data() const { return _shader_code.get_code_data(); }

}; // class CodeProvider



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CODEPROVIDER_H
