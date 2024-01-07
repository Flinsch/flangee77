#pragma once
#ifndef F77_TESTS_XL7_GRAPHICS_H
#define F77_TESTS_XL7_GRAPHICS_H

#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>

#include <CoreLabs/filesystem.h>

#include <TestLabs/TestSuite.h>



namespace cl7 {
    template <> inline
    cl7::string to_string(const xl7::graphics::shaders::ShaderCode::Language& shader_language)
    {
        switch ( shader_language )
        {
        case xl7::graphics::shaders::ShaderCode::Language::Unknown: return TEXT("#Unknown");
        case xl7::graphics::shaders::ShaderCode::Language::Bytecode: return TEXT("#Bytecode");
        //case xl7::graphics::shaders::ShaderCode::Language::Assembly: return TEXT("#Assembly");
        case xl7::graphics::shaders::ShaderCode::Language::HighLevel: return TEXT("#HighLevel");
        default:
            assert( false );
        }
        return TEXT("#Unknown");
    }
}



TESTLABS_CASE( TEXT("XiaoLabs:  graphics:  compile shaders") )
{
    xl7::graphics::impl::shared::shaders::D3DShaderCompiler shader_compiler;

    const cl7::string file_path = cl7::filesystem::get_working_directory() + TEXT("assets/shaders/shader.hlsl");

    struct Entry
    {
        cl7::astring entry_point;
        cl7::astring target;
        xl7::graphics::shaders::ParameterTable parameter_table;
    } entry;

    const std::vector<Entry> container {
        { "mainVS", "vs_3_0", { { "WorldViewProjection", { xl7::graphics::shaders::ParameterType::Float, 0, 4, 4, 4, 1 } } } },
        { "mainPS", "ps_3_0", { { "BaseColor", { xl7::graphics::shaders::ParameterType::Float, 0, 1, 1, 4, 1 } } } },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        xl7::graphics::shaders::ShaderCode bytecode = shader_compiler.compile_hlsl_code( file_path, {}, entry.entry_point, entry.target );
        xl7::graphics::shaders::ParameterTable parameter_table = shader_compiler.build_parameter_table( bytecode );

        TESTLABS_CHECK_EQ( bytecode.get_language(), xl7::graphics::shaders::ShaderCode::Language::Bytecode );
        TESTLABS_CHECK_EQ( parameter_table.size(), entry.parameter_table.size() );

        for ( const auto& p : entry.parameter_table )
        {
            const auto it = parameter_table.find( p.first );
            TESTLABS_ASSERT( it != parameter_table.end() );
            TESTLABS_CHECK_EQ( unsigned(p.second.type), unsigned(it->second.type) );
            TESTLABS_CHECK_EQ( p.second.register_index, it->second.register_index );
            TESTLABS_CHECK_EQ( p.second.register_count, it->second.register_count );
            TESTLABS_CHECK_EQ( p.second.row_count, it->second.row_count );
            TESTLABS_CHECK_EQ( p.second.column_count, it->second.column_count );
            TESTLABS_CHECK_EQ( p.second.element_count, it->second.element_count );
        }
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_H
