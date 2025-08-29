#include "D3DShaderCompiler.h"

#include "../errors.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/strings/codec.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include <d3dcompiler.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



namespace xl7::graphics::impl::shared::shaders {



    struct Include final
        : public ID3DInclude
    {
        struct ParentEntry
        {
            cl7::u8string directory;
            cl7::u8string code;
        };

        cl7::u8string _root_directory;
        std::unordered_map<const void*, ParentEntry> _parent_entries;

        Include(const cl7::u8string& path)
            : _root_directory(Include::directory(path))
        {
        }

        __declspec(nothrow) HRESULT __stdcall Open(D3D_INCLUDE_TYPE include_type, const char* filename, const void* parent_data, const void** data_out, unsigned* bytes_out) override
        {
            try
            {
                return _open_impl(include_type, filename, parent_data, data_out, bytes_out);
            }
            catch (...)
            {
                return E_FAIL;
            }
        }

        __declspec(nothrow) HRESULT __stdcall Close(const void* data) override
        {
            return _close_impl(data);
        }

        static cl7::u8string directory(const cl7::u8string& path)
        {
            if (std::filesystem::is_directory(path))
                return path;
            const size_t pos = path.find_last_of(u8"/\\");
            if (pos != cl7::u8string::npos)
                return path.substr(0, pos + 1);
            return {};
        }

        static cl7::u8string filename(const cl7::u8string& path)
        {
            if (!std::filesystem::is_regular_file(path))
            {
                LOG_ERROR(path + u8" does not refer to a regular file.");
                return {};
            }
            const size_t pos = path.find_last_of(u8"/\\");
            if (pos != cl7::u8string::npos)
                return path.substr(pos + 1);
            return path;
        }

        static cl7::u8string read_source_code(const cl7::u8string& file_path)
        {
            std::ifstream file(std::filesystem::path(cl7::strings::codec::reinterpret_utf8(file_path)), std::ios::in | std::ios::binary | std::ios::ate);
            if (!file.is_open())
            {
                LOG_ERROR(u8"Shader file \"" + Include::filename(file_path) + u8"\" could not be opened. Does it exist?");
                return {};
            }

            const auto size = static_cast<size_t>(file.tellg());
            file.seekg(0, std::ios::beg);

            cl7::u8string code(size, 0);
            if (!file.read(reinterpret_cast<char*>(code.data()), static_cast<std::streamsize>(size)))
            {
                LOG_ERROR(u8"Shader file \"" + Include::filename(file_path) + u8"\" could not be read.");
                return {};
            }

            return code;
        }

    private:

        HRESULT _open_impl(D3D_INCLUDE_TYPE include_type, const char* filename, const void* parent_data, const void** data_out, unsigned* bytes_out)
        {
            assert(include_type == D3D_INCLUDE_LOCAL);

            auto it = _parent_entries.find(parent_data);
            const cl7::u8string& directory = it == _parent_entries.end() ? _root_directory : it->second.directory;
            cl7::u8string file_path = directory + cl7::u8string(cl7::strings::codec::reinterpret_utf8(filename));
            std::ranges::replace(file_path, u8'/', u8'\\');

            cl7::u8string code = Include::read_source_code(file_path);
            if (code.empty())
                return E_FAIL;

            *data_out = static_cast<const void*>(code.data());
            *bytes_out = static_cast<unsigned>(code.size());

            _parent_entries.emplace(*data_out, ParentEntry{.directory = Include::directory(file_path), .code = std::move(code)});

            assert(*data_out == _parent_entries[*data_out].code.data());

            return S_OK;
        }

        HRESULT _close_impl(const void* data)
        {
            auto it = _parent_entries.find(data);
            assert(it != _parent_entries.end());
            _parent_entries.erase(it);

            return S_OK;
        }
    }; // struct Include



    /**
     * Compiles HLSL code from the specified file into bytecode for a given target.
     * The file path is also used to resolve any #include directives. If an error
     * occurs, an object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const cl7::u8string& file_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::u8string& entry_point, const cl7::u8string& target)
    {
        cl7::u8string source_code = Include::read_source_code(file_path);

        return compile_hlsl_code({xl7::graphics::shaders::ShaderCode::Language::HighLevel, cl7::make_byte_view(source_code)}, Include::directory(file_path), compile_options, entry_point, target);
    }

    /**
     * Compiles the given HLSL code into bytecode for a given target. The include
     * path is used to resolve any #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const xl7::graphics::shaders::ShaderCode& hlsl_code, const cl7::u8string& include_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::u8string& entry_point, const cl7::u8string& target)
    {
        if (hlsl_code.get_language() != xl7::graphics::shaders::ShaderCode::Language::HighLevel)
        {
            LOG_ERROR(u8"The given code does not appear to be in HLSL.");
            return {};
        }

        if (hlsl_code.get_code_data().empty())
        {
            LOG_ERROR(u8"The given HLSL code is empty.");
            return {};
        }

        if (entry_point.empty())
        {
            LOG_ERROR(u8"No shader entry point specified.");
            return {};
        }

        if (target.empty())
        {
            LOG_ERROR(u8"No shader target specified.");
            return {};
        }

        Include include(include_path);

        std::vector<D3D_SHADER_MACRO> macros;
        macros.reserve(compile_options.macro_definitions.size() + 1);
        for (const auto& [name, definition] : compile_options.macro_definitions)
            macros.push_back({.Name = reinterpret_cast<const char*>(name.c_str()), .Definition = reinterpret_cast<const char*>(definition.c_str())});
        macros.push_back({.Name = nullptr, .Definition = nullptr});

        unsigned flags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
#ifdef _DEBUG
        flags |= D3DCOMPILE_DEBUG;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        wrl::ComPtr<ID3DBlob> bytecode_blob;
        wrl::ComPtr<ID3DBlob> error_blob;

        HRESULT hresult = ::D3DCompile(
            hlsl_code.get_code_data().data(),
            hlsl_code.get_code_data().size(),
            nullptr,
            macros.data(),
            &include,
            reinterpret_cast<const char*>(entry_point.c_str()),
            reinterpret_cast<const char*>(target.c_str()),
            flags,
            0,
            &bytecode_blob,
            &error_blob);

        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d_result(hresult, u8"::D3DCompile"));

            cl7::u8string error_messages{static_cast<cl7::u8char_t*>(error_blob->GetBufferPointer()), error_blob->GetBufferSize()};
            for (size_t offset = 0, p = 0; offset < error_messages.length(); offset = p + 1)
            {
                p = error_messages.find(u8'\n', offset);
                if (p == cl7::u8string::npos)
                {
                    LOG_ERROR(error_messages.substr(offset));
                    break;
                }
                LOG_ERROR(error_messages.substr(offset, p - offset));
            }

            return {};
        }

        return {xl7::graphics::shaders::ShaderCode::Language::Bytecode, cl7::byte_view(static_cast<std::byte*>(bytecode_blob->GetBufferPointer()), bytecode_blob->GetBufferSize())};
    }



} // namespace xl7::graphics::impl::shared::shaders
