#include "D3DShaderReflection.h"

#include "../errors.h"

#include <CoreLabs/logging.h>

#include <d3dcompiler.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;

#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "dxguid.lib" )



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    bool D3DShaderReflection::reflect(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out)
    {
        if ( bytecode.get_language() != xl7::graphics::shaders::ShaderCode::Language::Bytecode )
        {
            LOG_ERROR( TEXT("The given code does not appear to be bytecode.") );
            return false;
        }

        if ( bytecode.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The given bytecode is empty.") );
            return false;
        }

        wrl::ComPtr<ID3D11ShaderReflection> d3d_shader_reflection;
        HRESULT hresult = ::D3DReflect(
            bytecode.get_code_data().data(),
            bytecode.get_code_data().size(),
            ::IID_ID3D11ShaderReflection,
            &d3d_shader_reflection );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("::D3DReflect") ) );
            return false;
        }

        auto& constant_buffer_declarations_out = reflection_result_out.constant_buffer_declarations;
        auto& texture_sampler_declarations_out = reflection_result_out.texture_sampler_declarations;

        D3D11_SHADER_DESC d3d_shader_desc;
        hresult = d3d_shader_reflection->GetDesc( &d3d_shader_desc );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11ShaderReflection::GetDesc") ) );
            return false;
        }

        for ( unsigned cbuffer_index = 0; cbuffer_index < d3d_shader_desc.ConstantBuffers; ++cbuffer_index )
        {
            ID3D11ShaderReflectionConstantBuffer* d3d_cbuffer_reflection = d3d_shader_reflection->GetConstantBufferByIndex( cbuffer_index );
            assert( d3d_cbuffer_reflection );
            if ( !d3d_cbuffer_reflection )
                continue;

            D3D11_SHADER_BUFFER_DESC d3d_shader_buffer_desc;
            hresult = d3d_cbuffer_reflection->GetDesc( &d3d_shader_buffer_desc );

            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11ShaderReflectionConstantBuffer::GetDesc") ) );
                continue;
            }

            if ( d3d_shader_buffer_desc.Type != D3D_CT_CBUFFER )
                continue;

            constant_buffer_declarations_out.emplace_back( xl7::graphics::shaders::ConstantBufferDeclaration{ d3d_shader_buffer_desc.Name, cbuffer_index, {} } );
            auto& constant_declarations_out = constant_buffer_declarations_out.back().layout.constant_declarations;

            for ( unsigned variable_index = 0; variable_index < d3d_shader_buffer_desc.Variables; ++variable_index )
            {
                ID3D11ShaderReflectionVariable* d3d_variable_reflection = d3d_cbuffer_reflection->GetVariableByIndex( variable_index );
                assert( d3d_variable_reflection );
                if ( !d3d_variable_reflection )
                    continue;

                D3D11_SHADER_VARIABLE_DESC d3d_shader_variable_desc;
                hresult = d3d_variable_reflection->GetDesc( &d3d_shader_variable_desc );

                if ( FAILED(hresult) )
                {
                    LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11ShaderReflectionVariable::GetDesc") ) );
                    continue;
                }

                ID3D11ShaderReflectionType* d3d_shader_variable_type = d3d_variable_reflection->GetType();
                assert( d3d_shader_variable_type );
                if ( !d3d_shader_variable_type )
                    continue;

                D3D11_SHADER_TYPE_DESC d3d_shader_type_desc;
                hresult = d3d_shader_variable_type->GetDesc( &d3d_shader_type_desc );

                if ( FAILED(hresult) )
                {
                    LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11ShaderReflectionType::GetDesc") ) );
                    continue;
                }

                xl7::graphics::shaders::ConstantType constant_type;
                switch ( d3d_shader_type_desc.Type )
                {
                    case D3D_SVT_BOOL:
                    case D3D_SVT_INT:
                    case D3D_SVT_FLOAT:
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantType::Bool ) == D3D_SVT_BOOL );
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantType::Int ) == D3D_SVT_INT );
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantType::Float ) == D3D_SVT_FLOAT );
                        constant_type = static_cast<xl7::graphics::shaders::ConstantType>( d3d_shader_type_desc.Type );
                        break;
                    default:
                        assert( false );
                        constant_type = xl7::graphics::shaders::ConstantType(-1);
                } // switch parameter type

                assert( constant_type != xl7::graphics::shaders::ConstantType(-1) );
                if ( constant_type == xl7::graphics::shaders::ConstantType(-1) )
                    continue;

                xl7::graphics::shaders::ConstantClass constant_class;
                switch ( d3d_shader_type_desc.Class )
                {
                    case D3D_SVC_SCALAR:
                    case D3D_SVC_VECTOR:
                    case D3D_SVC_MATRIX_ROWS:
                    case D3D_SVC_MATRIX_COLUMNS:
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantClass::Scalar ) == D3D_SVC_SCALAR );
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantClass::Vector ) == D3D_SVC_VECTOR );
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantClass::MatrixRows ) == D3D_SVC_MATRIX_ROWS );
                        static_assert( static_cast<unsigned>( xl7::graphics::shaders::ConstantClass::MatrixColumns ) == D3D_SVC_MATRIX_COLUMNS );
                        constant_class = static_cast<xl7::graphics::shaders::ConstantClass>( d3d_shader_type_desc.Class );
                        break;
                    default:
                        assert( false );
                        constant_class = xl7::graphics::shaders::ConstantClass(-1);
                } // switch parameter type

                assert( constant_class != xl7::graphics::shaders::ConstantClass(-1) );
                if ( constant_class == xl7::graphics::shaders::ConstantClass(-1) )
                    continue;

                xl7::graphics::shaders::ConstantDeclaration constant_declaration;
                constant_declaration.name = d3d_shader_variable_desc.Name;
                constant_declaration.constant_type = constant_type;
                constant_declaration.constant_class = constant_class;
                constant_declaration.row_count = d3d_shader_type_desc.Rows;
                constant_declaration.column_count = d3d_shader_type_desc.Columns;
                constant_declaration.element_count = (std::max)( d3d_shader_type_desc.Elements, 1u );
                constant_declaration.offset = d3d_shader_variable_desc.StartOffset;
                constant_declaration.size = d3d_shader_variable_desc.Size;

                constant_declarations_out.emplace_back( std::move(constant_declaration) );
            } // for each variable

            constant_buffer_declarations_out.back().layout.sort_and_adjust_padded_sizes();
        } // for each cbuffer

        for ( unsigned resource_index = 0; resource_index < d3d_shader_desc.BoundResources; ++resource_index )
        {
            D3D11_SHADER_INPUT_BIND_DESC d3d_shader_input_bind_desc;
            hresult = d3d_shader_reflection->GetResourceBindingDesc( resource_index, &d3d_shader_input_bind_desc );

            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11ShaderReflection::GetResourceBindingDesc") ) );
                continue;
            }

            if ( d3d_shader_input_bind_desc.Type != D3D_SIT_SAMPLER )
                continue;

            xl7::graphics::shaders::TextureSamplerDeclaration texture_sampler_declaration;
            texture_sampler_declaration.name = d3d_shader_input_bind_desc.Name;
            texture_sampler_declaration.index = d3d_shader_input_bind_desc.BindPoint;
            texture_sampler_declaration.element_count = d3d_shader_input_bind_desc.BindCount;

            texture_sampler_declarations_out.emplace_back( std::move(texture_sampler_declaration) );
        } // for each (sampler) resource

        return true;
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
