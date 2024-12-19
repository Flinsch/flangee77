#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CODEDATAPROVIDER_H
#define XL7_GRAPHICS_SHADERS_CODEDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"

#include "./ShaderCode.h"
#include "./CompileOptions.h"



namespace xl7::graphics::shaders {



class CodeDataProvider
    : public resources::DefaultDataProvider
{

public:
    CodeDataProvider(const ShaderCode* shader_code, const CompileOptions* compile_options);
    ~CodeDataProvider() override = default;



    /** Returns a reference to the shader code. */
    const ShaderCode& get_shader_code() const { return *_shader_code; }
    /** Returns a reference to the compile options. */
    const CompileOptions& get_compile_options() const { return *_compile_options; }

    /** Returns the language in which the code is given. */
    ShaderCode::Language get_language() const { return _shader_code->get_language(); }
    /** Returns the actual code data. */
    const cl7::byte_vector& get_code_data() const { return _shader_code->get_code_data(); }



private:
    /** A reference to the shader code. */
    const ShaderCode* _shader_code;
    /** A reference to the compile options. */
    const CompileOptions* _compile_options;

}; // class CodeDataProvider



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CODEDATAPROVIDER_H
