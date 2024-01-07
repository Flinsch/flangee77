#include "D3DShaderCompiler.h"

#include "../errors.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

#include <algorithm>
#include <vector>
#include <unordered_map>

#include <d3dcompiler.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;

#pragma comment( lib, "d3dcompiler.lib" )



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace shaders {



    struct Include final
        : public ID3DInclude
    {
        Include(const cl7::string& file_path)
            : _root_directory( Include::directory( file_path ) )
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

        static cl7::string directory(const cl7::string& file_path)
        {
            const size_t pos = file_path.find_last_of( TEXT("/\\") );
            if ( pos != cl7::string::npos )
                return file_path.substr( 0, pos + 1 );
            return {};
        }

        static cl7::string filename(const cl7::string& file_path)
        {
            const size_t pos = file_path.find_last_of( TEXT("/\\") );
            if ( pos != cl7::string::npos )
                return file_path.substr( pos + 1 );
            return file_path;
        }

        static cl7::astring read_source_code(const cl7::string& file_path)
        {
            cl7::aifstream file( file_path, std::ios::in | std::ios::binary | std::ios::ate );
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
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const cl7::string& file_path, const std::map<cl7::astring, cl7::astring>& defines, const cl7::astring& entry_point, const cl7::astring& target)
    {
        cl7::astring source_code = Include::read_source_code( file_path );

        return compile_hlsl_code( { xl7::graphics::shaders::ShaderCode::Language::HighLevel, cl7::byte_span( reinterpret_cast<std::byte*>( source_code.data() ), source_code.size() ) }, file_path, defines, entry_point, target );
    }

    /**
     * Compiles the given HLSL code into bytecode for a given target. The file path
     * is used to resolve any #include directives. If an error occurs, an object
     * with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode D3DShaderCompiler::compile_hlsl_code(const xl7::graphics::shaders::ShaderCode& hlsl_code, const cl7::string& file_path, const std::map<cl7::astring, cl7::astring>& defines, const cl7::astring& entry_point, const cl7::astring& target)
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

        Include include( file_path );

        std::vector<D3D_SHADER_MACRO> macros;
        macros.reserve( defines.size() + 1 );
        for ( const auto& p : defines )
            macros.push_back( { p.first.c_str(), p.second.c_str() } );
        macros.push_back( { nullptr, nullptr } );

        unsigned flags = 0;
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

        return { xl7::graphics::shaders::ShaderCode::Language::Bytecode, cl7::byte_span( static_cast<std::byte*>( bytecode_blob->GetBufferPointer() ), bytecode_blob->GetBufferSize() ) };
    }

    /**
     * Builds a parameter table based on the specified bytecode.
     */
    xl7::graphics::shaders::ParameterTable D3DShaderCompiler::build_parameter_table(const xl7::graphics::shaders::ShaderCode& bytecode)
    {
        if ( bytecode.get_language() != xl7::graphics::shaders::ShaderCode::Language::Bytecode )
        {
            LOG_ERROR( TEXT("The given code does not appear to be bytecode.") );
            return {};
        }

        if ( bytecode.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The given bytecode is empty.") );
            return {};
        }

        // Inspired by:
        // https://www.gamedev.net/forums/topic/648016-replacement-for-id3dxconstanttable/

        struct Header
        {
            uint32_t Size;
            uint32_t Creator;
            uint32_t Version;
            uint32_t Constants;
            uint32_t ConstantInfo;
            uint32_t Flags;
            uint32_t Target;
        };

        struct Info
        {
            uint32_t Name;
            uint16_t RegisterSet;
            uint16_t RegisterIndex;
            uint16_t RegisterCount;
            uint16_t Reserved;
            uint32_t TypeInfo;
            uint32_t DefaultValue;
        };

        struct Type
        {
            uint16_t Class;
            uint16_t Type;
            uint16_t Rows;
            uint16_t Columns;
            uint16_t Elements;
            uint16_t StructMembers;
            uint32_t StructMemberInfo;
        };

        constexpr uint32_t SIO_COMMENT = 0x0000fffe;
        constexpr uint32_t SIO_END = 0x0000ffff;
        constexpr uint32_t SI_OPCODE_MASK = 0x0000ffff;
        constexpr uint32_t SI_COMMENTSIZE_MASK = 0x7fff0000;
        constexpr uint32_t CTAB_CONSTANT = 0x42415443;

        assert( bytecode.get_code_data().size() % 4 == 0 );
        const uint32_t* const base_ptr = reinterpret_cast<const uint32_t*>( bytecode.get_code_data().data() );
        for ( size_t ofs = 0; ofs < bytecode.get_code_data().size(); ++ofs )
        {
            const uint32_t* const ptr = base_ptr + ofs;

            // Break if end of code.
            if ( *ptr == SIO_END )
                break;

            // Skip if no comment.
            if ( (*ptr & SI_OPCODE_MASK) != SIO_COMMENT )
                continue;

            // Skip comment if not constant/parameter table.
            const size_t comment_size = static_cast<size_t>( (*ptr & SI_COMMENTSIZE_MASK) >> 16 );
            if ( *(ptr + 1) != CTAB_CONSTANT )
            {
                ofs += comment_size;
                continue;
            }

            const char* const ctab_ptr = reinterpret_cast<const char*>( ptr + 2 );
            const size_t ctab_size = static_cast<size_t>( comment_size - 1 ) * 4;

            const Header* const cheader = reinterpret_cast<const Header*>( ctab_ptr );
            if ( ctab_size < sizeof(Header) || cheader->Size != sizeof(Header) )
            {
                LOG_ERROR( TEXT("Bad shader constant/parameter table header size.") );
                return {};
            }

            const Info* const cinfo = reinterpret_cast<const Info*>( ctab_ptr + cheader->ConstantInfo );

            xl7::graphics::shaders::ParameterTable parameter_table;

            for ( uint32_t i = 0; i < cheader->Constants; ++i )
            {
                const Type* const ctype = reinterpret_cast<const Type*>( ctab_ptr + cinfo[i].TypeInfo );

                xl7::graphics::shaders::ParameterType parameter_type;
                switch ( ctype->Type )
                {
                    case 1: // D3DXPT_BOOL
                        parameter_type = xl7::graphics::shaders::ParameterType::Bool;
                        break;
                    case 2: // D3DXPT_INT
                        parameter_type = xl7::graphics::shaders::ParameterType::Int;
                        break;
                    case 3: // D3DXPT_FLOAT
                        parameter_type = xl7::graphics::shaders::ParameterType::Float;
                        break;
                    case 10: // D3DXPT_SAMPLER
                    case 11: // D3DXPT_SAMPLER1D
                    case 12: // D3DXPT_SAMPLER2D
                    case 13: // D3DXPT_SAMPLER3D
                    case 14: // D3DXPT_SAMPLERCUBE
                        parameter_type = xl7::graphics::shaders::ParameterType::Sampler;
                        break;
                    default:
                        assert( false );
                        parameter_type = xl7::graphics::shaders::ParameterType::Invalid;
                } // switch parameter type

                assert( parameter_type != xl7::graphics::shaders::ParameterType::Invalid );
                if ( parameter_type == xl7::graphics::shaders::ParameterType::Invalid )
                    continue;

                xl7::graphics::shaders::ParameterDesc parameter_desc;
                parameter_desc.type = parameter_type;
                parameter_desc.register_index = static_cast<unsigned>( cinfo[i].RegisterIndex );
                parameter_desc.register_count = static_cast<unsigned>( cinfo[i].RegisterCount );

                parameter_desc.row_count = static_cast<unsigned>( ctype->Rows );
                parameter_desc.column_count = static_cast<unsigned>( ctype->Columns );
                parameter_desc.element_count = static_cast<unsigned>( ctype->Elements );

                parameter_table.emplace( cl7::astring{ ctab_ptr + cinfo[i].Name }, parameter_desc );
            } // for each constant

            return parameter_table;
        } // for ...

        // End of code reached without hitting comments sections.
        // This should be considered an error (or at least a warning).
        //LOG_ERROR( TEXT("The shader parameter table could not be created.") );
        LOG_WARNING( TEXT("The shader parameter table could not be created.") );
        return {};
    }



} // namespace shaders
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7
