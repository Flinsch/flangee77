#include "ShaderCompiler.h"

#include "../GraphicsSystem.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::shaders {



    ShaderCompiler::ShaderCompiler(Shader::Type shader_type, IncludeHandler* include_handler)
        : _shader_type(shader_type)
        , _include_handler(include_handler)
    {
        switch (_shader_type)
        {
        case Shader::Type::VertexShader:
            _shader_profile = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.max_vertex_shader_profile;
            break;
        case Shader::Type::PixelShader:
            _shader_profile = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.max_pixel_shader_profile;
            break;
        default:
            assert(false);
        }
    }

    ShaderCompiler::ShaderCompiler(Shader::Type shader_type, cl7::Version shader_profile, IncludeHandler* include_handler)
        : _shader_type(shader_type)
        , _shader_profile(std::move(shader_profile))
        , _include_handler(include_handler)
    {
    }



    /**
     * Compiles high-level code from the specified file into bytecode. The file path
     * is also used to resolve any (local) #include directives. If an error occurs,
     * an object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector ShaderCompiler::compile_source_code(const cl7::u8string& file_path, const CompileOptions& compile_options, const cl7::astring& entry_point)
    {
        if (file_path.empty())
        {
            LOG_ERROR(u8"The specified high-level shader code file path is empty.");
            return {};
        }

        if (entry_point.empty())
        {
            LOG_ERROR(u8"No shader entry point specified.");
            return {};
        }

        return _compile_source_code(file_path, compile_options, entry_point);

    }

    /**
     * Compiles the given high-level code into bytecode. The specified include path
     * is used to resolve any (local) #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector ShaderCompiler::compile_source_code(const cl7::u8string& source_code, const cl7::u8string& include_path, const CompileOptions& compile_options, const cl7::astring& entry_point)
    {
        if (source_code.empty())
        {
            LOG_ERROR(u8"The given high-level code is empty.");
            return {};
        }

        if (entry_point.empty())
        {
            LOG_ERROR(u8"No shader entry point specified.");
            return {};
        }

        return _compile_source_code(source_code, include_path, compile_options, entry_point);
    }



    /**
     * Compiles high-level code from the specified file into bytecode. The file path
     * is also used to resolve any (local) #include directives. If an error occurs,
     * an object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector ShaderCompiler::_compile_source_code(const cl7::u8string& file_path, const CompileOptions& compile_options, const cl7::astring& entry_point)
    {
        cl7::u8string source_code = _include_handler->read_source_code(file_path);

        return compile_source_code(source_code, _include_handler->directory(file_path), compile_options, entry_point);
    }



} // namespace xl7::graphics::shaders
