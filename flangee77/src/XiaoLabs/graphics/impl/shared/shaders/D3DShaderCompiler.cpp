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

        const std::vector<cl7::u8string>* _include_directories;
        graphics::shaders::IncludeHandler* _include_handler;

        explicit Include(const cl7::u8string& path, graphics::shaders::IncludeHandler* include_handler)
            : Include(path, nullptr, include_handler)
        {
        }

        explicit Include(const cl7::u8string& path, const std::vector<cl7::u8string>* include_directories, graphics::shaders::IncludeHandler* include_handler)
            : _include_directories(include_directories)
            , _include_handler(include_handler)
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

    private:

        cl7::u8string _try_path(const cl7::u8string& directory, const cl7::u8string& filename) const
        {
            if (directory.empty())
                return {};
            cl7::u8string path = this->directory(directory);
            if (path.empty())
                return {};
            if (path.back() != u8'\\' && path.back() != u8'/')
                path += u8'\\';
            path += filename;
            std::ranges::replace(path, u8'/', u8'\\');
            return std::filesystem::is_regular_file(path) ? path : cl7::u8string{};
        }

        HRESULT _open_impl(D3D_INCLUDE_TYPE include_type, const char* filename, const void* parent_data, const void** data_out, unsigned* bytes_out)
        {
            assert(include_type == D3D_INCLUDE_LOCAL);

            const cl7::u8string fname = cl7::u8string(cl7::text::codec::reinterpret_utf8(filename));

            auto it = _parent_entries.find(parent_data);
            const cl7::u8string& primary_dir = it == _parent_entries.end() ? _root_directory : it->second.directory;

            // 1. Try relative to current file (or root directory derived from shader file path).
            cl7::u8string file_path = _try_path(primary_dir, fname);

            // 2. Try per-compilation include directories (CompileOptions::include_directories).
            if (file_path.empty() && _include_directories)
            {
                for (const auto& dir : *_include_directories)
                {
                    file_path = _try_path(dir, fname);
                    if (!file_path.empty())
                        break;
                }
            }

            // 3. Try the "global" include root (IncludeHandler::set_include_root).
            if (file_path.empty())
                file_path = _try_path(_include_handler->get_include_root(), fname);

            if (file_path.empty())
            {
                LOG_ERROR(u8"\"" + fname + u8"\" could not be found in any include search path.");
                return E_FAIL;
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

        if (include_path.empty() && compile_options.include_directories.empty() && get_include_handler()->get_include_root().empty())
            LOG_WARNING(u8"No include search path defined; shader #include directives may fail to resolve.");

        Include include(include_path, &compile_options.include_directories, get_include_handler());

        const bool is_legacy = get_shader_profile().major < 4;

        std::vector<D3D_SHADER_MACRO> macros;
        macros.reserve(compile_options.macro_definitions.size() + 2);
        for (const auto& [name, definition] : compile_options.macro_definitions)
            macros.push_back({.Name = name.c_str(), .Definition = definition.c_str()});
        if (is_legacy)
            macros.push_back({.Name = "D3D9_COMPAT", .Definition = "1"});
        macros.push_back({.Name = nullptr, .Definition = nullptr});

        unsigned flags = is_legacy ? D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY : 0;
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
