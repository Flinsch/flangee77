#include "D3DShaderCompiler.h"

#include "../errors.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

#include <filesystem>

#include <algorithm>
#include <vector>
#include <unordered_map>

#include <d3dcompiler.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace shaders {



    struct Include final
        : public ID3DInclude
    {
        Include(const cl7::string& path)
            : _root_directory( Include::directory( path ) )
            , _parent_entries()
        {
        }

        const cl7::string _root_directory;
        struct ParentEntry
        {
            cl7::string directory;
            cl7::astring code;
        };
        std::unordered_map<const void*, ParentEntry> _parent_entries;

        static cl7::string directory(const cl7::string& path)
        {
            if ( std::filesystem::is_directory( path ) )
                return path;
            const size_t pos = path.find_last_of( TEXT("/\\") );
            if ( pos != cl7::string::npos )
                return path.substr( 0, pos + 1 );
            return {};
        }

        static cl7::string filename(const cl7::string& path)
        {
            if ( !std::filesystem::is_regular_file( path ) )
            {
                LOG_ERROR( path + TEXT(" does not refer to a regular file.") );
                return {};
            }
            const size_t pos = path.find_last_of( TEXT("/\\") );
            if ( pos != cl7::string::npos )
                return path.substr( pos + 1 );
            return path;
        }

        static cl7::astring read_source_code(const cl7::string& file_path)
        {
            cl7::aifstream file( file_path.data(), std::ios::in | std::ios::binary | std::ios::ate );
            if ( !file.is_open() )
            {
                LOG_ERROR( TEXT("Shader file \"") + Include::filename( file_path ) + TEXT("\" could not be opened. Does it exist?") );
                return {};
            }

            const size_t size = static_cast<size_t>( file.tellg() );
            file.seekg( 0, std::ios::beg );

            cl7::astring code( size, 0 );
            if ( !file.read( code.data(), static_cast<std::streamsize>( size ) ) )
            {
                LOG_ERROR( TEXT("Shader file \"") + Include::filename( file_path ) + TEXT("\" could not be read.") );
                return {};
            }

            return code;
        }

        __declspec(nothrow) HRESULT __stdcall Open(D3D_INCLUDE_TYPE include_type, const cl7::achar_type* filename, const void* parent_data, const void** data_out, unsigned* bytes_out) override
        {
            assert( include_type == D3D_INCLUDE_LOCAL );

            auto it = _parent_entries.find( parent_data );
            const cl7::string& directory = it == _parent_entries.end() ? _root_directory : it->second.directory;
            cl7::string file_path = directory + cl7::strings::from_ascii( filename );
            std::replace( file_path.begin(), file_path.end(), TEXT('/'), TEXT('\\') );

            cl7::astring code = Include::read_source_code( file_path );
            if ( code.empty() )
                return E_FAIL;

            *data_out = static_cast<const void*>( &code[0] );
            *bytes_out = static_cast<unsigned>( code.size() );

            _parent_entries.emplace( *data_out, ParentEntry{ Include::directory( file_path ), std::move(code) } );

            assert( *data_out == &_parent_entries[ *data_out ].code[0] );

            return S_OK;
        }

        __declspec(nothrow) HRESULT __stdcall Close(const void* data) override
        {
            auto it = _parent_entries.find( data );
            assert( it != _parent_entries.end() );
            _parent_entries.erase( it );

            return S_OK;
        }
    }; // struct Include



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Compiles HLSL code from the specified file into bytecode for a given target.
     * The file path is also used to resolve any #include directives. If an error
     * occurs, an object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const cl7::string& file_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point, const cl7::astring& target)
    {
        cl7::astring source_code = Include::read_source_code( file_path );

        return compile_hlsl_code( { xl7::graphics::shaders::ShaderCode::Language::HighLevel, cl7::byte_view( reinterpret_cast<std::byte*>( source_code.data() ), source_code.size() ) }, Include::directory( file_path ), compile_options, entry_point, target );
    }

    /**
     * Compiles the given HLSL code into bytecode for a given target. The include
     * path is used to resolve any #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const xl7::graphics::shaders::ShaderCode& hlsl_code, const cl7::string& include_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point, const cl7::astring& target)
    {
        if ( hlsl_code.get_language() != xl7::graphics::shaders::ShaderCode::Language::HighLevel )
        {
            LOG_ERROR( TEXT("The given code does not appear to be in HLSL.") );
            return {};
        }

        if ( hlsl_code.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The given HLSL code is empty.") );
            return {};
        }

        if ( entry_point.empty() )
        {
            LOG_ERROR( TEXT("No shader entry point specified.") );
            return {};
        }

        if ( target.empty() )
        {
            LOG_ERROR( TEXT("No shader target specified.") );
            return {};
        }

        Include include( include_path );

        std::vector<D3D_SHADER_MACRO> macros;
        macros.reserve( compile_options.macro_definitions.size() + 1 );
        for ( const auto& p : compile_options.macro_definitions )
            macros.push_back( { p.first.c_str(), p.second.c_str() } );
        macros.push_back( { nullptr, nullptr } );

        unsigned flags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
#ifdef _DEBUG
        flags |= D3DCOMPILE_DEBUG;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        wrl::ComPtr<ID3DBlob> bytecode_blob;
        wrl::ComPtr<ID3DBlob> error_blob;

        HRESULT hresult = D3DCompile(
            hlsl_code.get_code_data().data(),
            hlsl_code.get_code_data().size(),
            nullptr,
            &macros[0],
            &include,
            entry_point.c_str(),
            target.c_str(),
            flags,
            0,
            &bytecode_blob,
            &error_blob );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d_result( hresult, TEXT("::D3DCompile") ) );

            cl7::string error_messages( cl7::strings::from_ascii( cl7::astring_view( static_cast<cl7::achar_type*>( error_blob->GetBufferPointer() ), error_blob->GetBufferSize() ) ) );
            for ( size_t offset = 0, p = 0; offset < error_messages.length(); offset = p + 1 )
            {
                p = error_messages.find( TEXT('\n'), offset );
                if ( p == error_messages.npos )
                {
                    LOG_ERROR( error_messages.substr( offset ) );
                    break;
                }
                LOG_ERROR( error_messages.substr( offset, p - offset ) );
            }

            return {};
        }

        return { xl7::graphics::shaders::ShaderCode::Language::Bytecode, cl7::byte_view( static_cast<std::byte*>( bytecode_blob->GetBufferPointer() ), bytecode_blob->GetBufferSize() ) };
    }



} // namespace shaders
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7
