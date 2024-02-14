#pragma once
#ifndef F77_TESTS_XL7_GRAPHICS_H
#define F77_TESTS_XL7_GRAPHICS_H

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>

#include <CoreLabs/filesystem.h>
#include <CoreLabs/strings.h>

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

    template <> inline
    cl7::string to_string(const xl7::graphics::Color& color) { return TEXT("rgba(") + cl7::to_string(color.get_r()) + TEXT(", ") + cl7::to_string(color.get_g()) + TEXT(", ") + cl7::to_string(color.get_b()) + TEXT(", ") + cl7::to_string(color.get_a()) + TEXT(")"); }
}



TESTLABS_CASE( TEXT("XiaoLabs:  graphics:  Color") )
{
    TESTLABS_CHECK_EQ( xl7::graphics::Color(), xl7::graphics::Color( 0.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( uint8_t(63), uint8_t(127), uint8_t(255) ), xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x78123456, xl7::graphics::ChannelOrder::ARGB ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x78563412, xl7::graphics::ChannelOrder::ABGR ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x56341278, xl7::graphics::ChannelOrder::BGRA ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 220.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x7f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 580.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x7f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 940.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x7f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( -140.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x7f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( -500.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x7f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 30.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x9f), uint8_t(0x3f) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 90.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x9f), uint8_t(0xff), uint8_t(0x3f) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 150.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0xff), uint8_t(0x9f) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 210.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x3f), uint8_t(0x9f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 270.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0x9f), uint8_t(0x3f), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsv( 330.0f, 0.75f, 1.0f ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x3f), uint8_t(0x9f) ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 220.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x7d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 580.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x7d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 940.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x7d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( -140.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x7d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( -500.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x7d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 30.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x9e), uint8_t(0x3d) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 90.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x9e), uint8_t(0xff), uint8_t(0x3d) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 150.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0xff), uint8_t(0x9e) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 210.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x3d), uint8_t(0x9e), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 270.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0x9e), uint8_t(0x3d), uint8_t(0xff) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color::from_hsl( 330.0f, 1.0f, 0.62f ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x3d), uint8_t(0x9e) ) );

    xl7::graphics::Color color1;
    xl7::graphics::Color color2{ 0.125f, 0.25f, 0.5f, 1.0f };
    color1.swap( color2 );
    TESTLABS_CHECK_EQ( color1, xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_r(), 0x12 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_g(), 0x34 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_b(), 0x56 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_a(), 0x78 );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_r( 0xff ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_g( 0xff ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0xff), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_b( 0xff ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0xff), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_a( 0xff ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0xff) ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_rgba32(), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ).to_uint32( xl7::graphics::ChannelOrder::RGBA ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_argb32(), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ).to_uint32( xl7::graphics::ChannelOrder::ARGB ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_abgr32(), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ).to_uint32( xl7::graphics::ChannelOrder::ABGR ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_bgra32(), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ).to_uint32( xl7::graphics::ChannelOrder::BGRA ) );

    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba_ptr()[0], 0.125f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba_ptr()[1], 0.25f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba_ptr()[2], 0.5f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba_ptr()[3], 1.0f );

    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( xl7::graphics::Color( 0x1f3f7fff, xl7::graphics::ChannelOrder::RGBA ).get_hue(), 0 ), 220.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( xl7::graphics::Color( 0x1f3f7fff, xl7::graphics::ChannelOrder::RGBA ).get_saturation(), 3 ), 0.756f );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( xl7::graphics::Color( 0x1f3f7fff, xl7::graphics::ChannelOrder::RGBA ).get_lightness(), 3 ), 0.310f );
    TESTLABS_CHECK_EQ_FLT( ml7::utilities::round( xl7::graphics::Color( 0x1f3f7fff, xl7::graphics::ChannelOrder::RGBA ).get_brightness(), 3 ), 0.498f );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( -1.0f, -2.0f, -3.0f, -4.0f ).saturated(), xl7::graphics::Color( 0.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( +1.0f, +2.0f, +3.0f, +4.0f ).saturated(), xl7::graphics::Color( 1.0f, 1.0f, 1.0f, 1.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( -1.0f, +2.0f, -3.0f, +4.0f ).saturated(), xl7::graphics::Color( 0.0f, 1.0f, 0.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( +xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ), xl7::graphics::Color( +0.125f, +0.25f, +0.5f, +1.0f ) );
    TESTLABS_CHECK_EQ( -xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ), xl7::graphics::Color( -0.125f, -0.25f, -0.5f, -1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) + xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ), xl7::graphics::Color( 0.375f, 0.75f, 1.5f, 3.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) - xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ), xl7::graphics::Color( -0.125f, -0.25f, -0.5f, -1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) * xl7::graphics::Color( 1.0f, 2.0f, 3.0f, 4.0f ), xl7::graphics::Color( 0.125f, 0.5f, 1.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) / xl7::graphics::Color( 0.125f, 0.5f, 2.0f, 5.0f ), xl7::graphics::Color( 1.0f, 0.5f, 0.25f, 0.2f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) * 2.0f, xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) / 2.0f, xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) );
    TESTLABS_CHECK_EQ( 2.0f * xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ), xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) += xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ), xl7::graphics::Color( 0.375f, 0.75f, 1.5f, 3.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) -= xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ), xl7::graphics::Color( -0.125f, -0.25f, -0.5f, -1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) *= xl7::graphics::Color( 1.0f, 2.0f, 3.0f, 4.0f ), xl7::graphics::Color( 0.125f, 0.5f, 1.5f, 4.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) /= xl7::graphics::Color( 0.125f, 0.5f, 2.0f, 5.0f ), xl7::graphics::Color( 1.0f, 0.5f, 0.25f, 0.2f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) *= 2.0f, xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) /= 2.0f, xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) );

    TESTLABS_CHECK( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) == xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) );
    TESTLABS_CHECK( !(xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) == xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f )) );
    TESTLABS_CHECK( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) != xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 2.0f ) );
    TESTLABS_CHECK( !(xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ) != xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f )) );
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
