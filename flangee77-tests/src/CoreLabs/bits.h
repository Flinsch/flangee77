#ifndef F77_TESTS_CL7_BITS_H
#define F77_TESTS_CL7_BITS_H

#include <CoreLabs/bits.h>

#include <MathLabs/functions.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  bits:  swap_bytes" )
{
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint8_t( 7 ) ), uint8_t( 7 ) );

    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint16_t(0x1234) ), uint16_t(0x3412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint32_t(0x12345678) ), uint32_t(0x78563412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint64_t(0x123456789abcdef0) ), uint64_t(0xf0debc9a78563412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( -1 ), -1 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( int32_t(0xff0000ff) ), 0xff0000ff );

    if constexpr (std::endian::native == std::endian::little)
    {
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian<std::endian::little>( 0x12345678 ), 0x78563412 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian<std::endian::big>( 0x12345678 ), 0x12345678 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian<std::endian::little>( 0x12345678 ), 0x12345678 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian<std::endian::big>( 0x12345678 ), 0x78563412 );
    }

    if constexpr (std::endian::native == std::endian::big)
    {
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian<std::endian::little>( 0x12345678 ), 0x12345678 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian<std::endian::big>( 0x12345678 ), 0x78563412 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian<std::endian::little>( 0x12345678 ), 0x78563412 );
        TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian<std::endian::big>( 0x12345678 ), 0x12345678 );
    }
}



TESTLABS_CASE( u8"CoreLabs:  bits:  float_to_half/half_to_float" )
{
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( 0.0f ) ), 0.0f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( 0.5f ) ), 0.5f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( 1.0f ) ), 1.0f );

    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -0.0f ) ), -0.0f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -0.5f ) ), -0.5f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -1.0f ) ), -1.0f );

    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( +INFINITY ) ), +INFINITY );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -INFINITY ) ), -INFINITY );

    float pos_nan = cl7::bits::half_to_float( cl7::bits::float_to_half( +NAN ) );
    float neg_nan = cl7::bits::half_to_float( cl7::bits::float_to_half( -NAN ) );
    TESTLABS_CHECK_GE( *(uint32_t*)&pos_nan & 0x7fffffff, 0x7f800000 );
    TESTLABS_CHECK_GE( *(uint32_t*)&neg_nan & 0x7fffffff, 0x7f800000 );

    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( +65504.0f ) ), +65504.0f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -65504.0f ) ), -65504.0f );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( +65536.0f ) ), +INFINITY );
    TESTLABS_CHECK_EQ( cl7::bits::half_to_float( cl7::bits::float_to_half( -65536.0f ) ), -INFINITY );
    TESTLABS_CHECK_EQ_STR( cl7::bits::half_to_float( cl7::bits::float_to_half( +0.0006f ) ), +0.0006f );
    TESTLABS_CHECK_EQ_STR( cl7::bits::half_to_float( cl7::bits::float_to_half( -0.0006f ) ), -0.0006f );
    TESTLABS_CHECK_EQ_FLT( cl7::bits::half_to_float( cl7::bits::float_to_half( +0.00006f ) ), +0.0f );
    TESTLABS_CHECK_EQ_FLT( cl7::bits::half_to_float( cl7::bits::float_to_half( -0.00006f ) ), -0.0f );
    TESTLABS_CHECK_EQ_STR( cl7::bits::half_to_float( cl7::bits::float_to_half( +0.000006f ) ), +0.0f );
    TESTLABS_CHECK_EQ_STR( cl7::bits::half_to_float( cl7::bits::float_to_half( -0.000006f ) ), -0.0f );
}



TESTLABS_CASE( u8"CoreLabs:  bits:  norm_to_fixed" )
{
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.0f, 8 ), 0x00 );
    //TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.5f, 8 ), 0x7f );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.5f, 8 ), 0x80 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 1.0f, 8 ), 0xff );

    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.0f, 32 ), 0x00000000 );
    //TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.5f, 32 ), 0x7fffffff );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.5f, 32 ), 0x80000000 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 1.0f, 32 ), 0xffffffff );

    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.0f, 1 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 1.0f, 1 ), 1 );

    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.000f, 2 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.333f, 2 ), 1 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 0.667f, 2 ), 2 );
    TESTLABS_CHECK_EQ( cl7::bits::norm_to_fixed( 1.000f, 2 ), 3 );
}

TESTLABS_CASE( u8"CoreLabs:  bits:  fixed_to_norm" )
{
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x00, 8 ), 2 ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x7f, 8 ), 2 ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x80, 8 ), 2 ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0xff, 8 ), 2 ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x00000000, 32 ), 2 ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x7fffffff, 32 ), 2 ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0x80000000, 32 ), 2 ), 0.5f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0xffffffff, 32 ), 2 ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0, 1 ), 1 ), 0.0f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 1, 1 ), 1 ), 1.0f );

    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 0, 2 ), 3 ), 0.000f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 1, 2 ), 3 ), 0.333f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 2, 2 ), 3 ), 0.667f );
    TESTLABS_CHECK_EQ( ml7::round( cl7::bits::fixed_to_norm( 3, 2 ), 3 ), 1.000f );
}

TESTLABS_CASE( u8"CoreLabs:  bits:  fixed_to_fixed" )
{
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0, 32, 32 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( -1, 32, 32 ), -1 );

    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0, 32, 1 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( -1, 32, 1 ), 1 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0, 32, 2 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( -1, 32, 2 ), 3 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0, 1, 32 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 1, 1, 32 ), -1 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0, 2, 32 ), 0 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 3, 2, 32 ), -1 );

    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x00, 8, 16 ), 0x0000 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0xff, 8, 16 ), 0xffff );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x7f, 8, 16 ), 0x7f7f );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x01, 8, 16 ), 0x0101 );

    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x0000, 16, 8 ), 0x00 );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0xffff, 16, 8 ), 0xff );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x7f7f, 16, 8 ), 0x7f );
    TESTLABS_CHECK_EQ( cl7::bits::fixed_to_fixed( 0x0101, 16, 8 ), 0x01 );
}



#endif // F77_TESTS_CL7_BITS_H
