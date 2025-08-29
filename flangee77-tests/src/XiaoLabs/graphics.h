#ifndef F77_TESTS_XL7_GRAPHICS_H
#define F77_TESTS_XL7_GRAPHICS_H

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/PixelBitKit.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>
#include <XiaoLabs/graphics/images/ImageResizer.h>
#include <XiaoLabs/graphics/impl/shared/shaders/D3DShaderCompiler.h>
#include <XiaoLabs/graphics/impl/direct3d9/shaders/D3DShaderReflection.h>
#if defined(_MSC_VER)
#include <XiaoLabs/graphics/impl/direct3d11/shaders/D3DShaderReflection.h>
#endif

#include <CoreLabs/filesystem.h>
#include <CoreLabs/strings/format.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



inline bool operator==(const xl7::graphics::PixelBitKit::Channel& lhs, const xl7::graphics::PixelBitKit::Channel& rhs)
{
    return
        lhs.index == rhs.index &&
        lhs.depth == rhs.depth &&
        lhs.offset == rhs.offset &&
        lhs.mask == rhs.mask &&
        lhs.mask0 == rhs.mask0;
}



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

    inline
    cl7::u8string to_string(const xl7::graphics::Color& color) { return u8"rgba(" + cl7::to_string(color.get_r()) + u8", " + cl7::to_string(color.get_g()) + u8", " + cl7::to_string(color.get_b()) + u8", " + cl7::to_string(color.get_a()) + u8")"; }

    inline
    cl7::u8string to_string(const xl7::graphics::PixelBitKit::DataType& data_type)
    {
        switch ( data_type )
        {
        case xl7::graphics::PixelBitKit::DataType::UNKNOWN: return u8"#UNKNOWN";
        case xl7::graphics::PixelBitKit::DataType::UNORM: return u8"#UNORM";
        case xl7::graphics::PixelBitKit::DataType::SNORM: return u8"#SNORM";
        case xl7::graphics::PixelBitKit::DataType::UINT: return u8"#UINT";
        case xl7::graphics::PixelBitKit::DataType::SINT: return u8"#SINT";
        case xl7::graphics::PixelBitKit::DataType::FLOAT: return u8"#FLOAT";
        default:
            assert( false );
        }
        return u8"#UNKNOWN";
    }

    inline
    cl7::u8string to_string(const xl7::graphics::PixelBitKit::Channel& channel) { return u8"{" + cl7::to_string(channel.index) + u8", " + cl7::to_string(channel.depth) + u8", " + cl7::to_string(channel.offset) + u8", " + (channel.mask ? cl7::strings::format::to_0xhex_lc(channel.mask) : u8"0") + u8", " + (channel.mask0 ? cl7::strings::format::to_0xhex_lc(channel.mask0) : u8"0") + u8"}"; }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  Color" )
{
    TESTLABS_CHECK_EQ( xl7::graphics::Color(), xl7::graphics::Color( 0.0f, 0.0f, 0.0f, 0.0f ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( uint8_t(63), uint8_t(127), uint8_t(255) ), xl7::graphics::Color( 0.25f, 0.5f, 1.0f, 1.0f ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x78563412, xl7::graphics::ChannelOrder::RGBA ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x56341278, xl7::graphics::ChannelOrder::ARGB ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::ABGR ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x78123456, xl7::graphics::ChannelOrder::BGRA ), xl7::graphics::Color( uint8_t(0x12), uint8_t(0x34), uint8_t(0x56), uint8_t(0x78) ) );

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

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_r(), 0x78 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_g(), 0x56 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_b(), 0x34 );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).get_a(), 0x12 );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_r( 0xff ), xl7::graphics::Color( uint8_t(0xff), uint8_t(0x56), uint8_t(0x34), uint8_t(0x12) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_g( 0xff ), xl7::graphics::Color( uint8_t(0x78), uint8_t(0xff), uint8_t(0x34), uint8_t(0x12) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_b( 0xff ), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0xff), uint8_t(0x12) ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).set_a( 0xff ), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0x34), uint8_t(0xff) ) );

    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_rgba32(), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0x34), uint8_t(0x12) ).to_uint32( xl7::graphics::ChannelOrder::RGBA ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_argb32(), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0x34), uint8_t(0x12) ).to_uint32( xl7::graphics::ChannelOrder::ARGB ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_abgr32(), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0x34), uint8_t(0x12) ).to_uint32( xl7::graphics::ChannelOrder::ABGR ) );
    TESTLABS_CHECK_EQ( xl7::graphics::Color( 0x12345678, xl7::graphics::ChannelOrder::RGBA ).to_bgra32(), xl7::graphics::Color( uint8_t(0x78), uint8_t(0x56), uint8_t(0x34), uint8_t(0x12) ).to_uint32( xl7::graphics::ChannelOrder::BGRA ) );

    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba()[0], 0.125f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba()[1], 0.25f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba()[2], 0.5f );
    TESTLABS_CHECK_EQ_FLT( xl7::graphics::Color( 0.125f, 0.25f, 0.5f, 1.0f ).get_rgba()[3], 1.0f );

    TESTLABS_CHECK_EQ_FLT( ml7::round( xl7::graphics::Color( 0xff7f3f1f, xl7::graphics::ChannelOrder::RGBA ).get_hue(), 0 ), 220.0f );
    TESTLABS_CHECK_EQ_FLT( ml7::round( xl7::graphics::Color( 0xff7f3f1f, xl7::graphics::ChannelOrder::RGBA ).get_saturation(), 3 ), 0.756f );
    TESTLABS_CHECK_EQ_FLT( ml7::round( xl7::graphics::Color( 0xff7f3f1f, xl7::graphics::ChannelOrder::RGBA ).get_lightness(), 3 ), 0.310f );
    TESTLABS_CHECK_EQ_FLT( ml7::round( xl7::graphics::Color( 0xff7f3f1f, xl7::graphics::ChannelOrder::RGBA ).get_brightness(), 3 ), 0.498f );

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



TESTLABS_CASE( u8"XiaoLabs:  graphics:  PixelBitKit" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 53 );


    TESTLABS_SUBCASE( u8"data type" )
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


    TESTLABS_SUBCASE( u8"channel count" )
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



TESTLABS_CASE( u8"XiaoLabs:  graphics:  ImageProcessor" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 53 );


    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;
        xl7::graphics::ChannelOrder channel_order;
        cl7::byte_vector packed_data;
    } entry;

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
        return     4;
    };


    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"pack_color/unpack_color", container, entry, cl7::to_string( entry.pixel_format ) + u8" " + cl7::to_string( entry.channel_order ) )
    {
        xl7::graphics::PixelBitKit pbk{ entry.pixel_format, entry.channel_order };
        size_t stride = static_cast<size_t>( pbk.stride );

        cl7::byte_vector packed_data{ stride };
        xl7::graphics::images::ImageProcessor::pack_color( entry_color, entry.pixel_format, entry.channel_order, packed_data );
        xl7::graphics::Color color = xl7::graphics::images::ImageProcessor::unpack_color( entry.packed_data, entry.pixel_format, entry.channel_order );

        TESTLABS_CHECK_EQ( packed_data, entry.packed_data );

        if ( pbk.uniform_depth == 8 && pbk.channel_count == 4 ) // R8G8B8A8_UNORM
        {
            uint32_t packed_uint32 = *((const uint32_t*)(const uint8_t*)packed_data.data());
            xl7::graphics::Color color{ uint8_t(entry.packed_data[pbk.r.index]), uint8_t(entry.packed_data[pbk.g.index]), uint8_t(entry.packed_data[pbk.b.index]), uint8_t(entry.packed_data[pbk.a.index]) };
            TESTLABS_CHECK_EQ( packed_uint32, color.to_uint32( entry.channel_order ) );
        }

        for ( unsigned rgba = 0; rgba < 4; ++rgba )
        {
            unsigned depth = pbk.channels[ rgba ].depth;
            if ( !depth )
                continue;

            unsigned prec = _precision( depth );

            TESTLABS_CHECK_EQ( ml7::round( color[rgba], prec ), ml7::round( entry_color[rgba], prec ) );
        }
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  ImageConverter" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 53 );


    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;
        xl7::graphics::ChannelOrder channel_order;
    } entry1, entry2;

    std::vector<Entry> container;
    for ( unsigned pixel_format = 1; pixel_format <= static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ); ++pixel_format )
    {
        if ( xl7::graphics::PixelFormat( pixel_format ) == xl7::graphics::PixelFormat::R11G11B10_FLOAT )
            continue;
        for ( unsigned channel_order = 0; channel_order < 4; ++channel_order )
            container.push_back( { xl7::graphics::PixelFormat( pixel_format ), xl7::graphics::ChannelOrder( channel_order ) } );
    }

    const xl7::graphics::Color image_colors[4] = {
        { 0.9f, 0.3f, 0.1f, 0.5f },
        { 0.1f, 0.9f, 0.3f, 0.5f },
        { 0.3f, 0.1f, 0.9f, 0.5f },
        { 0.75f, 0.75f, 0.25f, 0.5f },
    };


    constexpr cl7::u8char_t rgbakeys[4] = { u8'R', u8'G', u8'B', u8'A' };

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
        return     4;
    };


    TESTLABS_SUBCASE_BATCH( u8"convert_image", container, entry1 )
    {
        xl7::graphics::PixelBitKit pbk1{ entry1.pixel_format, entry1.channel_order };
        size_t stride1 = static_cast<size_t>( pbk1.stride );

        cl7::byte_vector source_data{ stride1 * 4 };
        xl7::graphics::images::ImageConverter::pack_color( image_colors[0], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 0, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[1], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 1, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[2], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 2, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[3], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 3, stride1 } );

        xl7::graphics::images::Image source_image{ { entry1.pixel_format, entry1.channel_order, 2, 2, 1 }, source_data, true };

        TESTLABS_ASSERT_EQ( source_image.get_data().size(), 4 * stride1 );

        TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"convert_image", container, entry2, cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) )
        {
            xl7::graphics::PixelBitKit pbk2{ entry2.pixel_format, entry2.channel_order };
            size_t stride2 = static_cast<size_t>( pbk2.stride );

            xl7::graphics::images::Image target_image = xl7::graphics::images::ImageConverter::convert_image( source_image, entry2.pixel_format, entry2.channel_order );

            TESTLABS_CHECK( target_image.get_pixel_format() == entry2.pixel_format );
            TESTLABS_CHECK( target_image.get_channel_order() == entry2.channel_order );
            TESTLABS_CHECK_EQ( target_image.get_width(), 2 );
            TESTLABS_CHECK_EQ( target_image.get_height(), 2 );
            TESTLABS_CHECK_EQ( target_image.get_depth(), 1 );

            TESTLABS_CHECK_EQ( target_image.get_data().size(), target_image.get_desc().calculate_data_size() );

            TESTLABS_ASSERT_EQ( target_image.get_data().size(), 4 * stride2 );

            const auto& target_data = target_image.get_data();
            xl7::graphics::Color colors[4] = {
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 0, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 1, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 2, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 3, stride2 }, entry2.pixel_format, entry2.channel_order ),
            };

            for ( unsigned rgba = 0; rgba < 4; ++rgba )
            {
                TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) + u8"  (" + rgbakeys[rgba] + u8")" );

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
                        TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) + u8"  (" + rgbakeys[rgba_] + u8" -> " + rgbakeys[rgba] + u8")" );

                    unsigned depth1 = pbk1.channels[ rgba_ ].depth;
                    assert( depth1 );
                    unsigned depth2 = pbk2.channels[ rgba ].depth;

                    unsigned prec = ml7::min2( _precision( depth1 ), _precision( depth2 ) );

                    TESTLABS_CHECK_EQ( ml7::round( colors[0][rgba], prec ), ml7::round( image_colors[0][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[1][rgba], prec ), ml7::round( image_colors[1][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[2][rgba], prec ), ml7::round( image_colors[2][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[3][rgba], prec ), ml7::round( image_colors[3][rgba_], prec ) );
                }
            }
        }
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  ImageResizer" )
{
    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;

        unsigned source_width, source_height, source_depth;
        unsigned target_width, target_height, target_depth;

        xl7::graphics::images::ResamplingMethod resampling_method;

        cl7::byte_vector source_data;
        cl7::byte_vector target_data;
    } entry;

    const std::vector<Entry> container = {
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 0x7f ),  cl7::make_bytes( 0x7f ) },


        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 14 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  3, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 13,14,15 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 3, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 11,14,17 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 1, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 5,14,23 ) },

        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 14 ),        cl7::make_bytes( 14,14,14, 14,14,14, 14,14,14,  14,14,14, 14,14,14, 14,14,14,  14,14,14, 14,14,14, 14,14,14 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 1, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 13,14,15 ),  cl7::make_bytes( 13,14,15, 13,14,15, 13,14,15,  13,14,15, 13,14,15, 13,14,15,  13,14,15, 13,14,15, 13,14,15 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 3, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 11,14,17 ),  cl7::make_bytes( 11,11,11, 14,14,14, 17,17,17,  11,11,11, 14,14,14, 17,17,17,  11,11,11, 14,14,14, 17,17,17 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 3,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 5,14,23 ),   cl7::make_bytes( 5,5,5, 5,5,5, 5,5,5,  14,14,14, 14,14,14, 14,14,14,  23,23,23, 23,23,23, 23,23,23 ) },


        { xl7::graphics::PixelFormat::R8_UNORM,  4, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2,3,4 ),  cl7::make_bytes( 2,4 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  2, 2, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2, 3,4 ),  cl7::make_bytes( 3 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  2, 2, 2,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2, 3,4,  5,6, 7,8 ),  cl7::make_bytes( 5 ) },


        { xl7::graphics::PixelFormat::R32_FLOAT,  1, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  3, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  3, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40 ),  cl7::make_bytes( 0xab,0xaa,0xaa,0x3f, 0xab,0xaa,0x2a,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  4, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  4, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40 ),  cl7::make_bytes( 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x60,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  5, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  5, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40, 0x00,0x00,0xa0,0x40 ),  cl7::make_bytes( 0x66,0x66,0xe6,0x3f, 0x66,0x66,0x86,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 2, 1,  3, 3, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40,  0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40,  0x00,0x00,0x00,0x40, 0x00,0x00,0x20,0x40, 0x00,0x00,0x40,0x40,  0x00,0x00,0x40,0x40, 0x00,0x00,0x60,0x40, 0x00,0x00,0x80,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  3, 3, 1,  2, 2, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40,  0x00,0x00,0x80,0x40, 0x00,0x00,0xa0,0x40, 0x00,0x00,0xc0,0x40,  0x00,0x00,0xe0,0x40, 0x00,0x00,0x00,0x41, 0x00,0x00,0x10,0x41 ),  cl7::make_bytes( 0x55,0x55,0x15,0x40, 0xab,0xaa,0x6a,0x40,  0xab,0xaa,0xca,0x40, 0x55,0x55,0xf5,0x40 ) },
    };


    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"resize_image", container, entry, cl7::to_string( entry.pixel_format ) + u8" " + cl7::to_string(entry.source_width) + u8"x" + cl7::to_string(entry.source_height) + u8"x" + cl7::to_string(entry.source_depth) + u8" -> " + cl7::to_string(entry.target_width) + u8"x" + cl7::to_string(entry.target_height) + u8"x" + cl7::to_string(entry.target_depth) )
    {
        xl7::graphics::PixelBitKit pbk{ entry.pixel_format, xl7::graphics::ChannelOrder::RGBA };
        size_t stride = static_cast<size_t>( pbk.stride );

        xl7::graphics::images::Image source_image{ { entry.pixel_format, xl7::graphics::ChannelOrder::RGBA, entry.source_width, entry.source_height, entry.source_depth }, entry.source_data, true };

        TESTLABS_CHECK_EQ( source_image.get_data().size(), source_image.get_desc().calculate_data_size() );

        TESTLABS_ASSERT_EQ( source_image.get_data().size(), entry.source_data.size() );

        xl7::graphics::images::Image target_image = xl7::graphics::images::ImageResizer::resize_image( source_image, entry.target_width, entry.target_height, entry.target_depth, entry.resampling_method );

        TESTLABS_CHECK( target_image.get_pixel_format() == entry.pixel_format );
        TESTLABS_CHECK( target_image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( target_image.get_width(), entry.target_width );
        TESTLABS_CHECK_EQ( target_image.get_height(), entry.target_height );
        TESTLABS_CHECK_EQ( target_image.get_depth(), entry.target_depth );

        TESTLABS_CHECK_EQ( target_image.get_data().size(), target_image.get_desc().calculate_data_size() );

        TESTLABS_ASSERT_EQ( target_image.get_data().size(), entry.target_data.size() );

        if ( pbk.data_type == xl7::graphics::PixelBitKit::DataType::FLOAT )
        {
            for ( size_t i = 0; i < target_image.get_data().size(); i += stride )
            {
                assert( i % sizeof(float) == 0 );
                size_t j = i / sizeof(float);
                TESTLABS_CHECK_EQ( ml7::round( ((const float*)target_image.get_data().data())[ j ], 4 ), ml7::round( ((const float*)entry.target_data.data())[ j ], 4 ) );
            }
        }
        else
        {
            cl7::byte_vector target_image_data{ target_image.get_data().data(), target_image.get_data().data() + target_image.get_data().size() };

            TESTLABS_CHECK_EQ( target_image_data, entry.target_data );
        }
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  compile shaders" )
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
        cl7::u8string entry_point;
        cl7::u8string target;
        xl7::graphics::shaders::ReflectionResult reflection_result;
    } entry;

    const std::vector<Entry> container {
        { ImplType::Direct3D9, u8"shader.hlsl", u8"mainVS", u8"vs_3_0", { { { u8"", 0, { { { u8"WorldViewProjection", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::MatrixColumns, 4, 4, 1, 0, 64, 64 } } } } }, {} } },
        { ImplType::Direct3D9, u8"shader.hlsl", u8"mainPS", u8"ps_3_0", { { { u8"", 0, { { { u8"BaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 0, 16, 16 } } } } }, {} } },
#if defined(_MSC_VER)
        { ImplType::Direct3D11, u8"shader.hlsl", u8"mainVS", u8"vs_5_0", { { { u8"VertexConstants", 0, { { { u8"WorldViewProjection", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::MatrixColumns, 4, 4, 1, 0, 64, 64 } } } } }, {} } },
        { ImplType::Direct3D11, u8"shader.hlsl", u8"mainPS", u8"ps_5_0", { { { u8"PixelConstants", 0, { { { u8"BaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 0, 16, 16 } } } } }, {} } },
#endif
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        const cl7::u8string file_path = cl7::filesystem::get_working_directory() + u8"assets/shaders/" + entry.filename;

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



#endif // F77_TESTS_XL7_GRAPHICS_H
