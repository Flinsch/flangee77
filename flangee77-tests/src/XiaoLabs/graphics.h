#pragma once
#ifndef F77_TESTS_XL7_GRAPHICS_H
#define F77_TESTS_XL7_GRAPHICS_H

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/PixelBitKit.h>
#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>

#include <CoreLabs/filesystem.h>
#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>



bool operator == (const xl7::graphics::PixelBitKit::Channel& lhs, const xl7::graphics::PixelBitKit::Channel& rhs)
{
    return
        lhs.index == rhs.index &&
        lhs.depth == rhs.depth &&
        lhs.offset == rhs.offset &&
        lhs.mask == rhs.mask &&
        lhs.mask0 == rhs.mask0;
}



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

    template <> inline
    cl7::string to_string(const xl7::graphics::PixelBitKit::DataType& data_type)
    {
        switch ( data_type )
        {
        case xl7::graphics::PixelBitKit::DataType::UNKNOWN: return TEXT("#UNKNOWN");
        case xl7::graphics::PixelBitKit::DataType::UNORM: return TEXT("#UNORM");
        case xl7::graphics::PixelBitKit::DataType::SNORM: return TEXT("#SNORM");
        case xl7::graphics::PixelBitKit::DataType::UINT: return TEXT("#UINT");
        case xl7::graphics::PixelBitKit::DataType::SINT: return TEXT("#SINT");
        case xl7::graphics::PixelBitKit::DataType::FLOAT: return TEXT("#FLOAT");
        default:
            assert( false );
        }
        return TEXT("#UNKNOWN");
    }

    template <> inline
    cl7::string to_string(const xl7::graphics::PixelBitKit::Channel& channel) { return TEXT("{") + cl7::to_string(channel.index) + TEXT(", ") + cl7::to_string(channel.depth) + TEXT(", ") + cl7::to_string(channel.offset) + TEXT(", ") + (channel.mask ? cl7::strings::to_0xhex(channel.mask, TEXT('a')) : TEXT("0")) + TEXT(", ") + (channel.mask0 ? cl7::strings::to_0xhex(channel.mask0, TEXT('a')) : TEXT("0")) + TEXT("}"); }
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



TESTLABS_CASE( TEXT("XiaoLabs:  graphics:  PixelBitKit") )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_CHECK_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 53 );


    TESTLABS_SUBCASE( TEXT("data type") )
    {
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UNORM );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SNORM );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R10G10B10A2_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::UINT );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::SINT );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R11G11B10_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelBitKit::DataType::FLOAT );
    }


    TESTLABS_SUBCASE( TEXT("channel count") )
    {
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R11G11B10_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R8G8B8A8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R10G10B10A2_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelBitKit( xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
    }


    {
        struct Entry
        {
            xl7::graphics::PixelFormat pixel_format;
            unsigned stride;
            xl7::graphics::PixelBitKit::Channel r, g, b, a;
        };

        const std::vector<Entry> container {
            { xl7::graphics::PixelFormat::R8_UNORM, 1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_SNORM, 1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_UINT,  1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_SINT,  1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R16_UNORM,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_SNORM,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_UINT,     2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_SINT,     2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_FLOAT,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R32_UINT,     4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R32_SINT,     4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R32_FLOAT,    4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R8G8_UNORM,   2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_SNORM,   2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_UINT,    2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_SINT,    2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },

            { xl7::graphics::PixelFormat::R16G16_UNORM, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_SNORM, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_UINT,  4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_SINT,  4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_FLOAT, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },

            { xl7::graphics::PixelFormat::R32G32_UINT,  8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },
            { xl7::graphics::PixelFormat::R32G32_SINT,  8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },
            { xl7::graphics::PixelFormat::R32G32_FLOAT, 8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },

            { xl7::graphics::PixelFormat::R4G4B4X4_UNORM,   2, { 0, 4, 0, 0x000f, 0x000f }, { 1, 4, 4, 0x00f0, 0x000f }, { 2, 4,  8, 0x0f00, 0x000f }, {} },
            { xl7::graphics::PixelFormat::R5G5B5X1_UNORM,   2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 5, 5, 0x03e0, 0x001f }, { 2, 5, 10, 0x7c00, 0x001f }, {} },
            { xl7::graphics::PixelFormat::R5G6B5_UNORM,     2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 6, 5, 0x07e0, 0x003f }, { 2, 5, 11, 0xf800, 0x001f }, {} },

            { xl7::graphics::PixelFormat::R8G8B8_UNORM, 3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_SNORM, 3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_UINT,  3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_SINT,  3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },

            { xl7::graphics::PixelFormat::R8G8B8X8_UNORM,   4, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },

            { xl7::graphics::PixelFormat::R11G11B10_FLOAT,  4, { 0, 11, 0, 0x000007ff, 0x000007ff }, { 1, 11, 11, 0x003ff800, 0x000007ff }, { 2, 10, 22, 0xffc00000, 0x000003ff }, {} },

            { xl7::graphics::PixelFormat::R32G32B32_UINT,   12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },
            { xl7::graphics::PixelFormat::R32G32B32_SINT,   12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },
            { xl7::graphics::PixelFormat::R32G32B32_FLOAT,  12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },

            { xl7::graphics::PixelFormat::R4G4B4A4_UNORM,   2, { 0, 4, 0, 0x000f, 0x000f }, { 1, 4, 4, 0x00f0, 0x000f }, { 2, 4,  8, 0x0f00, 0x000f }, { 3, 4, 12, 0xf000, 0x000f } },
            { xl7::graphics::PixelFormat::R5G5B5A1_UNORM,   2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 5, 5, 0x03e0, 0x001f }, { 2, 5, 10, 0x7c00, 0x001f }, { 3, 1, 15, 0x8000, 0x0001 } },

            { xl7::graphics::PixelFormat::R8G8B8A8_UNORM,   4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_SNORM,   4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_UINT,    4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_SINT,    4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },

            { xl7::graphics::PixelFormat::R10G10B10A2_UNORM,    4, { 0, 10, 0, 0x000003ff, 0x000003ff }, { 1, 10, 10, 0x000ffc00, 0x000003ff }, { 2, 10, 20, 0x3ff00000, 0x000003ff }, { 3, 2, 30, 0xc0000000, 0x00000003 } },
            { xl7::graphics::PixelFormat::R10G10B10A2_UINT,     4, { 0, 10, 0, 0x000003ff, 0x000003ff }, { 1, 10, 10, 0x000ffc00, 0x000003ff }, { 2, 10, 20, 0x3ff00000, 0x000003ff }, { 3, 2, 30, 0xc0000000, 0x00000003 } },

            { xl7::graphics::PixelFormat::R16G16B16A16_UNORM,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_SNORM,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_UINT,    8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_SINT,    8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },

            { xl7::graphics::PixelFormat::R32G32B32A32_UINT,    16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },
            { xl7::graphics::PixelFormat::R32G32B32A32_SINT,    16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },
            { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT,   16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },

            { xl7::graphics::PixelFormat::A8_UNORM, 1, {}, {}, {}, { 0, 8, 0, 0xff, 0xff } },
        };

        for ( size_t i = 0; i < container.size(); ++i )
        {
            const Entry& entry = container[ i ];

            xl7::graphics::PixelBitKit pbk{ entry.pixel_format, xl7::graphics::ChannelOrder::RGBA };

            TESTLABS_CHECK_EQ( pbk.stride, entry.stride );

            TESTLABS_CHECK_EQ( pbk.r, entry.r );
            TESTLABS_CHECK_EQ( pbk.g, entry.g );
            TESTLABS_CHECK_EQ( pbk.b, entry.b );
            TESTLABS_CHECK_EQ( pbk.a, entry.a );
        }
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
