#pragma once
#ifndef F77_TESTS_XL7_GRAPHICS_H
#define F77_TESTS_XL7_GRAPHICS_H

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/PixelBitKit.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>
#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>

#include <CoreLabs/filesystem.h>
#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



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



TESTLABS_CASE( TEXT("XiaoLabs:  graphics:  ImageConverter") )
{
    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;
        xl7::graphics::ChannelOrder channel_order;
        cl7::byte_vector packed_data;
    } entry, entry1, entry2;

    const std::vector<Entry> container {
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x33 ) },

        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x32 ) },

        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66, 0x33 ) },

        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33, 0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33,0x33, 0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32, 0x66,0x36 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x32, 0x66,0x36 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x36, 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x36, 0x66,0x32 ) },

        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x03 | 0x60, 0x09 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x03 | 0x60, 0x09 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x09 | 0x60, 0x03 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x09 | 0x60, 0x03 ) },

        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10), (6<<0 | 12<<5 | 19<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10), (6<<0 | 12<<5 | 19<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10), (19<<0 | 12<<5 | 6<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10), (19<<0 | 12<<5 | 6<<10) >> 8 ) },

        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 25<<5 | 19<<11), (6<<0 | 25<<5 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (6<<0 | 25<<5 | 19<<11), (6<<0 | 25<<5 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (19<<0 | 25<<5 | 6<<11), (19<<0 | 25<<5 | 6<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 25<<5 | 6<<11), (19<<0 | 25<<5 | 6<<11) >> 8 ) },

        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x99, 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33 ) },

        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66, 0x99, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x99, 0x66, 0x33, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33, 0x00 ) },

        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x03 | 0x60, 0x09 | 0xc0 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x0c | 0x30, 0x06 | 0x90 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x0c | 0x90, 0x06 | 0x30 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x09 | 0x60, 0x03 | 0xc0 ) },

        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10 | 1<<15), (6<<0 | 12<<5 | 19<<10 | 1<<15) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (1<<0 | 6<<1 | 12<<6 | 19<<11), (1<<0 | 6<<1 | 12<<6 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (1<<0 | 19<<1 | 12<<6 | 6<<11), (1<<0 | 19<<1 | 12<<6 | 6<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10 | 1<<15), (19<<0 | 12<<5 | 6<<10 | 1<<15) >> 8 ) },

        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99, 0xcc ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc, 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc, 0x99, 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33, 0xcc ) },

        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (204<<0 | 409<<10 | 614<<20 | 3<<30), (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 8, (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 16, (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (3<<0 | 204<<2 | 409<<12 | 614<<22), (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 8, (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 16, (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (3<<0 | 614<<2 | 409<<12 | 204<<22), (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 8, (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 16, (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (614<<0 | 409<<10 | 204<<20 | 3<<30), (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 8, (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 16, (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 24 ) },

        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33, 0x66,0x66, 0x99,0x99, 0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc,0xcc, 0x33,0x33, 0x66,0x66, 0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc,0xcc, 0x99,0x99, 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99,0x99, 0x66,0x66, 0x33,0x33, 0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32, 0x66,0x36, 0xcc,0x38, 0x66,0x3a ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x3a, 0x66,0x32, 0x66,0x36, 0xcc,0x38 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x3a, 0xcc,0x38, 0x66,0x36, 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcc,0x38, 0x66,0x36, 0x66,0x32, 0x66,0x3a ) },

        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99, 0x00,0xcd,0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x00,0xcd,0xcc,0xcc, 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x00,0xcd,0xcc,0xcc, 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33, 0x00,0xcd,0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0x4c,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3f, 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3f, 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0x4c,0x3f ) },

        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcc ) },
    };

    const xl7::graphics::Color entry_color( 0.2f, 0.4f, 0.6f, 0.8f );

    const xl7::graphics::Color image_colors[4] = {
        { 0.9f, 0.3f, 0.1f, 0.5f },
        { 0.1f, 0.9f, 0.3f, 0.5f },
        { 0.3f, 0.1f, 0.9f, 0.5f },
        { 0.75f, 0.75f, 0.25f, 0.5f },
    };

    constexpr cl7::char_type rgbakeys[4] = { TEXT('R'), TEXT('G'), TEXT('B'), TEXT('A') };

    auto _precision = [](unsigned depth) -> unsigned {
        if ( depth <= 2 )
            return 0;
        if ( depth <= 6 )
            return 1;
        if ( depth <= 10 )
            return 2;
        if ( depth <= 16 )
            return 3;
        // => depth > 16
            return 4;
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("pack_color/unpack_color"), container, entry, cl7::to_string( entry.pixel_format ) + TEXT(" ") + cl7::to_string( entry.channel_order ) )
    {
        xl7::graphics::PixelBitKit pbk{ entry.pixel_format, entry.channel_order };
        size_t stride = static_cast<size_t>( pbk.stride );

        cl7::byte_vector packed_data{ stride };
        xl7::graphics::images::ImageConverter::pack_color( entry_color, entry.pixel_format, entry.channel_order, packed_data );
        xl7::graphics::Color color = xl7::graphics::images::ImageConverter::unpack_color( entry.packed_data, entry.pixel_format, entry.channel_order );

        TESTLABS_CHECK_EQ( packed_data, entry.packed_data );

        for ( unsigned rgba = 0; rgba < 4; ++rgba )
        {
            unsigned depth = pbk.channels[ rgba ].depth;
            if ( !depth )
                continue;

            unsigned prec = _precision( depth );

            TESTLABS_CHECK_EQ( ml7::utilities::round( color[rgba], prec ), ml7::utilities::round( entry_color[rgba], prec ) );
        }
    }

    TESTLABS_SUBCASE_BATCH( TEXT("pack_color/unpack_color"), container, entry1 )
    {
        xl7::graphics::PixelBitKit pbk1{ entry1.pixel_format, entry1.channel_order };
        size_t stride1 = static_cast<size_t>( pbk1.stride );

        cl7::byte_vector source_data{ stride1 * 4 };
        xl7::graphics::images::ImageConverter::pack_color( image_colors[0], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 0, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[1], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 1, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[2], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 2, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[3], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 3, stride1 } );

        xl7::graphics::images::Image source_image{ { entry1.pixel_format, entry1.channel_order, 2, 2, 1 }, source_data };

        TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( TEXT("convert_image"), container, entry2, cl7::to_string( entry1.pixel_format ) + TEXT(" ") + cl7::to_string( entry1.channel_order ) + TEXT(" -> ") + cl7::to_string( entry2.pixel_format ) + TEXT(" ") + cl7::to_string( entry2.channel_order ) )
        {
            xl7::graphics::PixelBitKit pbk2{ entry2.pixel_format, entry2.channel_order };
            size_t stride2 = static_cast<size_t>( pbk2.stride );

            xl7::graphics::images::Image target_image = xl7::graphics::images::ImageConverter::convert_image( source_image, entry2.pixel_format, entry2.channel_order );

            const cl7::byte_vector& target_data = target_image.get_data();
            xl7::graphics::Color colors[4] = {
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 0, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 1, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 2, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 3, stride2 }, entry2.pixel_format, entry2.channel_order ),
            };

            for ( unsigned rgba = 0; rgba < 4; ++rgba )
            {
                unsigned depth1 = pbk1.channels[ rgba ].depth;
                unsigned depth2 = pbk2.channels[ rgba ].depth;

                TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + TEXT(" ") + cl7::to_string( entry1.channel_order ) + TEXT(" -> ") + cl7::to_string( entry2.pixel_format ) + TEXT(" ") + cl7::to_string( entry2.channel_order ) + TEXT("  (") + rgbakeys[rgba] + TEXT(")") );

                constexpr signed rgba_map[5][5][4] = {
                    {
                        { -1, -1, -1,  3 }, // A    -> A
                        {  3,  3,  3, -1 }, // A    -> R
                        {  3,  3, -1, -1 }, // A    -> RG
                        {  3,  3,  3, -1 }, // A    -> RGB
                        { -1, -1, -1,  3 }, // A    -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // R    -> A
                        {  0,  0,  0, -1 }, // R    -> R
                        {  0,  0, -1, -1 }, // R    -> RG
                        {  0,  0,  0, -1 }, // R    -> RGB
                        {  0,  0,  0, -1 }, // R    -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // RG   -> A
                        {  0,  0,  0, -1 }, // RG   -> R
                        {  0,  1, -1, -1 }, // RG   -> RG
                        {  0,  1, -1, -1 }, // RG   -> RGB
                        {  0,  1, -1, -1 }, // RG   -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // RGB  -> A
                        {  0,  0,  0, -1 }, // RGB  -> R
                        {  0,  1, -1, -1 }, // RGB  -> RG
                        {  0,  1,  2, -1 }, // RGB  -> RGB
                        {  0,  1,  2, -1 }, // RGB  -> RGBA
                    },
                    {
                        { -1, -1, -1,  3 }, // RGBA -> A
                        {  0,  0,  0, -1 }, // RGBA -> R
                        {  0,  1, -1, -1 }, // RGBA -> RG
                        {  0,  1,  2, -1 }, // RGBA -> RGB
                        {  0,  1,  2,  3 }, // RGBA -> RGBA
                    },
                };

                auto _from_to = [](const xl7::graphics::PixelBitKit& pbk) -> unsigned {
                    if ( pbk.channel_count == 1 && pbk.a.depth )
                        return 0;
                    return pbk.channel_count;
                };

                signed mapped_rgba = rgba_map[ _from_to(pbk1) ][ _from_to(pbk2) ][ rgba ];
                if ( mapped_rgba < 0 )
                {
                    TESTLABS_CHECK_EQ( colors[0][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[1][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[2][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[3][rgba], xl7::graphics::Color::BLACK[rgba] );
                }
                else
                {
                    unsigned rgba_ = static_cast<unsigned>( mapped_rgba );
                    assert( rgba_ < 4 );

                    if ( rgba_ != rgba )
                        TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + TEXT(" ") + cl7::to_string( entry1.channel_order ) + TEXT(" -> ") + cl7::to_string( entry2.pixel_format ) + TEXT(" ") + cl7::to_string( entry2.channel_order ) + TEXT("  (") + rgbakeys[rgba_] + TEXT(" -> ") + rgbakeys[rgba] + TEXT(")") );

                    depth1 = pbk1.channels[ rgba_ ].depth;
                    assert( depth1 );

                    unsigned prec = ml7::utilities::min2( _precision( depth1 ), _precision( depth2 ) );

                    TESTLABS_CHECK_EQ( ml7::utilities::round( colors[0][rgba], prec ), ml7::utilities::round( image_colors[0][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::utilities::round( colors[1][rgba], prec ), ml7::utilities::round( image_colors[1][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::utilities::round( colors[2][rgba], prec ), ml7::utilities::round( image_colors[2][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::utilities::round( colors[3][rgba], prec ), ml7::utilities::round( image_colors[3][rgba_], prec ) );
                }
            }
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
