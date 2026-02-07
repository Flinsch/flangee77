#ifndef F77_TESTS_XL7_GRAPHICS_SHADERS_REFLECTION_H
#define F77_TESTS_XL7_GRAPHICS_SHADERS_REFLECTION_H

#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>
#include <XiaoLabs/graphics/impl/direct3d9/shaders/D3DShaderReflection.h>
#if defined(_MSC_VER)
#include <XiaoLabs/graphics/impl/direct3d11/shaders/D3DShaderReflection.h>
#endif

#include <CoreLabs/platform/filesystem.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const xl7::graphics::shaders::ShaderCode::Language& shader_language)
    {
        switch ( shader_language )
        {
        case xl7::graphics::shaders::ShaderCode::Language::Unknown: return u8"#Unknown";
        case xl7::graphics::shaders::ShaderCode::Language::Bytecode: return u8"#Bytecode";
        //case xl7::graphics::shaders::ShaderCode::Language::Assembly: return u8"#Assembly";
        case xl7::graphics::shaders::ShaderCode::Language::HighLevel: return u8"#HighLevel";
        default:
            assert( false );
        }
        return u8"#Unknown";
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  shaders:  reflection" )
{
    enum struct ImplType
    {
        Direct3D9,
        Direct3D11,
    };

    struct Entry
    {
        ImplType impl_type;
        cl7::u8string filename;
        cl7::astring entry_point;
        cl7::astring target;
        xl7::graphics::shaders::ReflectionResult reflection_result;
    } entry;

    const std::vector<Entry> container {
        { ImplType::Direct3D9, u8"shader.hlsl", "mainVS", "vs_3_0", { { { "", 0, { { { "WorldViewProjection", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::MatrixColumns, 4, 4, 1, 0, 64, 64 } } } } }, {} } },
        { ImplType::Direct3D9, u8"shader.hlsl", "mainPS", "ps_3_0", { { { "", 0, { { { "BaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 0, 16, 16 } } } } }, {} } },
#if defined(_MSC_VER)
        { ImplType::Direct3D11, u8"shader.hlsl", "mainVS", "vs_5_0", { { { "VertexConstants", 0, { { { "WorldViewProjection", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::MatrixColumns, 4, 4, 1, 0, 64, 64 } } } } }, {} } },
        { ImplType::Direct3D11, u8"shader.hlsl", "mainPS", "ps_5_0", { { { "PixelConstants", 0, { { { "BaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 0, 16, 16 } } } } }, {} } },
#endif
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        const cl7::u8string file_path = cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/" + entry.filename;

        xl7::graphics::shaders::ShaderCode bytecode = xl7::graphics::impl::shared::shaders::D3DShaderCompiler::compile_hlsl_code( file_path, {}, entry.entry_point, entry.target );
        xl7::graphics::shaders::ReflectionResult reflection_result;
        bool reflection_success;
        switch ( entry.impl_type )
        {
        case ImplType::Direct3D9:
            reflection_success = xl7::graphics::impl::direct3d9::shaders::D3DShaderReflection::reflect( bytecode, reflection_result );
            break;
#if defined(_MSC_VER)
        case ImplType::Direct3D11:
            reflection_success = xl7::graphics::impl::direct3d11::shaders::D3DShaderReflection::reflect( bytecode, reflection_result );
            break;
#endif
        default:
            assert( false );
        }

        auto& constant_buffer_declarations = reflection_result.constant_buffer_declarations;
        auto& texture_sampler_declarations = reflection_result.texture_sampler_declarations;

        TESTLABS_CHECK_EQ( bytecode.get_language(), xl7::graphics::shaders::ShaderCode::Language::Bytecode );
        TESTLABS_CHECK( reflection_success );
        TESTLABS_ASSERT_EQ( constant_buffer_declarations.size(), entry.reflection_result.constant_buffer_declarations.size() );
        TESTLABS_ASSERT_EQ( texture_sampler_declarations.size(), entry.reflection_result.texture_sampler_declarations.size() );

        for ( size_t i_cb = 0; i_cb < constant_buffer_declarations.size(); ++i_cb )
        {
            const auto& expected_cb = entry.reflection_result.constant_buffer_declarations[ i_cb ];
            auto& actual_cb = constant_buffer_declarations[ i_cb ];

            TESTLABS_CHECK_EQ( actual_cb.name, expected_cb.name );
            TESTLABS_CHECK_EQ( actual_cb.index, expected_cb.index );

            std::vector<xl7::graphics::shaders::ConstantDeclaration>& constant_declarations = actual_cb.layout.constant_declarations;

            TESTLABS_ASSERT_EQ( constant_declarations.size(), entry.reflection_result.constant_buffer_declarations[ i_cb ].layout.constant_declarations.size() );

            for ( size_t i_c = 0; i_c < constant_declarations.size(); ++i_c )
            {
                const auto& expected_c = entry.reflection_result.constant_buffer_declarations[ i_cb ].layout.constant_declarations[ i_c ];
                auto& actual_c = constant_declarations[ i_c ];

                TESTLABS_CHECK_EQ( actual_c.name, expected_c.name );
                TESTLABS_CHECK_EQ( unsigned(actual_c.constant_type), unsigned(expected_c.constant_type) );
                TESTLABS_CHECK_EQ( unsigned(actual_c.constant_class), unsigned(expected_c.constant_class) );
                TESTLABS_CHECK_EQ( actual_c.row_count, expected_c.row_count );
                TESTLABS_CHECK_EQ( actual_c.column_count, expected_c.column_count );
                TESTLABS_CHECK_EQ( actual_c.element_count, expected_c.element_count );
                TESTLABS_CHECK_EQ( actual_c.offset, expected_c.offset );
                TESTLABS_CHECK_EQ( actual_c.size, expected_c.size );
                TESTLABS_CHECK_EQ( actual_c.padded_size, expected_c.padded_size );
            } // for each constant "variable"
        } // for each cbuffer

        for ( size_t i_ts = 0; i_ts < texture_sampler_declarations.size(); ++i_ts )
        {
            const auto& expected_ts = entry.reflection_result.texture_sampler_declarations[ i_ts ];
            auto& actual_ts = texture_sampler_declarations[ i_ts ];

            TESTLABS_CHECK_EQ( actual_ts.name, expected_ts.name );
            TESTLABS_CHECK_EQ( actual_ts.index, expected_ts.index );
            TESTLABS_CHECK_EQ( actual_ts.element_count, expected_ts.element_count );
        } // for each texture/sampler
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_SHADERS_REFLECTION_H
