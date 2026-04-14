#ifndef XL7_GRAPHICS_SHADERS_SHADERCOMPILER_H
#define XL7_GRAPHICS_SHADERS_SHADERCOMPILER_H

#include "./Shader.h"
#include "./IncludeHandler.h"
#include "./CompileOptions.h"

#include <CoreLabs/Version.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



class ShaderCompiler
{

public:
    ShaderCompiler(Shader::Type shader_type, IncludeHandler* include_handler);
    ShaderCompiler(Shader::Type shader_type, cl7::Version shader_profile, IncludeHandler* include_handler);

    virtual ~ShaderCompiler() = default;



    /** Returns the type of the shader(s) to compile. */
    Shader::Type get_shader_type() const { return _shader_type; }
    /** Returns the shader profile to target. */
    const cl7::Version& get_shader_profile() const { return _shader_profile; }
    /** Returns the include handler to use. */
    IncludeHandler* get_include_handler() const { return _include_handler; }



    /**
     * Compiles high-level code from the specified file into bytecode. The file path
     * is also used to resolve any (local) #include directives. If an error occurs,
     * an object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector compile_source_code(const cl7::u8string& file_path, const CompileOptions& compile_options, const cl7::astring& entry_point);

    /**
     * Compiles the given high-level code into bytecode. The specified include path
     * is used to resolve any (local) #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector compile_source_code(const cl7::u8string& source_code, const cl7::u8string& include_path, const CompileOptions& compile_options, const cl7::astring& entry_point);



private:
    /**
     * Compiles high-level code from the specified file into bytecode. The file path
     * is also used to resolve any (local) #include directives. If an error occurs,
     * an object with an "unknown" language and empty data is returned.
     */
    virtual cl7::byte_vector _compile_source_code(const cl7::u8string& file_path, const CompileOptions& compile_options, const cl7::astring& entry_point);

    /**
     * Compiles the given high-level code into bytecode. The specified include path
     * is used to resolve any (local) #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    virtual cl7::byte_vector _compile_source_code(const cl7::u8string& source_code, const cl7::u8string& include_path, const CompileOptions& compile_options, const cl7::astring& entry_point) = 0;



    /** The type of the shader(s) to compile. */
    Shader::Type _shader_type;
    /** The shader profile to target. */
    cl7::Version _shader_profile;
    /** The include handler to use. */
    IncludeHandler* _include_handler;

}; // class ShaderCompiler



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADERCOMPILER_H
