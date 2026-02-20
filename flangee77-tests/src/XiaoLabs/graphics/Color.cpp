
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/Color.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const xl7::graphics::Color& color) { return u8"rgba(" + cl7::to_string(color.get_r()) + u8", " + cl7::to_string(color.get_g()) + u8", " + cl7::to_string(color.get_b()) + u8", " + cl7::to_string(color.get_a()) + u8")"; }
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
