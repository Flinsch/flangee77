#include "D3DShaderCompiler.h"

#include "../errors.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>

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
        : ID3DInclude
    {
        struct ParentEntry
        {
            cl7::u8string directory;
            cl7::u8string code;
        };

        cl7::u8string _root_directory;
        std::unordered_map<const void*, ParentEntry> _parent_entries;

        graphics::shaders::IncludeHandler* _include_handler;

        explicit Include(const cl7::u8string& path, graphics::shaders::IncludeHandler* include_handler)
            : _include_handler(include_handler)
        {
            _root_directory = this->directory(path);
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

        cl7::u8string directory(const cl7::u8string& path) const
        {
            if (std::filesystem::is_directory(path))
                return path;

            return _include_handler->directory(path);
        }

        cl7::u8string filename(const cl7::u8string& path) const
        {
            if (!std::filesystem::is_regular_file(path))
            {
                LOG_ERROR(path + u8" does not refer to a regular file.");
                return {};
            }

            return _include_handler->filename(path);
        }

    private:

        HRESULT _open_impl(D3D_INCLUDE_TYPE include_type, const char* filename, const void* parent_data, const void** data_out, unsigned* bytes_out)
        {
            assert(include_type == D3D_INCLUDE_LOCAL);

            auto it = _parent_entries.find(parent_data);
            const cl7::u8string& directory = it == _parent_entries.end() ? _root_directory : it->second.directory;
            cl7::u8string file_path = directory + cl7::u8string(cl7::text::codec::reinterpret_utf8(filename));
            std::ranges::replace(file_path, u8'/', u8'\\');

            if (!std::filesystem::is_regular_file(file_path))
            {
                LOG_ERROR(u8"\"" + this->filename(file_path) + u8"\" does not refer to a regular file.");
                return {};
            }

            cl7::u8string code = _include_handler->read_source_code(file_path);
            if (code.empty())
                return E_FAIL;

            const void* data = code.data();

            _parent_entries.emplace(data, ParentEntry{.directory = this->directory(file_path), .code = std::move(code)});

            const auto& parent_entry = _parent_entries[data];

            assert(data == parent_entry.code.data());

            *data_out = parent_entry.code.data();
            *bytes_out = static_cast<unsigned>(parent_entry.code.size());

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
     * Compiles the given high-level code into bytecode. The specified include path
     * is used to resolve any (local) #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    cl7::byte_vector D3DShaderCompiler::_compile_source_code(const cl7::u8string& hlsl_code, const cl7::u8string& include_path, const graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point)
    {
        if (hlsl_code.empty())
        {
            LOG_ERROR(u8"The given HLSL code is empty.");
            return {};
        }

        if (entry_point.empty())
        {
            LOG_ERROR(u8"No shader entry point specified.");
            return {};
        }

        cl7::astring target;
        switch (get_shader_type())
        {
        case graphics::shaders::Shader::Type::VertexShader:
            target = "vs_" + std::to_string(get_shader_profile().major) + "_" + std::to_string(get_shader_profile().minor);
            break;
        case graphics::shaders::Shader::Type::PixelShader:
            target = "ps_" + std::to_string(get_shader_profile().major) + "_" + std::to_string(get_shader_profile().minor);
            break;
        default:
            assert(false);
        }

        Include include(include_path, get_include_handler());

        std::vector<D3D_SHADER_MACRO> macros;
        macros.reserve(compile_options.macro_definitions.size() + 1);
        for (const auto& [name, definition] : compile_options.macro_definitions)
            macros.push_back({.Name = name.c_str(), .Definition = definition.c_str()});
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
            hlsl_code.data(),
            hlsl_code.size(),
            nullptr,
            macros.data(),
            &include,
            entry_point.c_str(),
            target.c_str(),
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

        return cl7::to_bytes(cl7::byte_view(static_cast<std::byte*>(bytecode_blob->GetBufferPointer()), bytecode_blob->GetBufferSize()));
    }



} // namespace xl7::graphics::impl::shared::shaders
