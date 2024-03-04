#pragma once
#ifndef F77_TESTS_CL7_UTILITIES_H
#define F77_TESTS_CL7_UTILITIES_H

#include <CoreLabs/bits.h>

#include <TestLabs/TestSuite.h>



TESTLABS_CASE( TEXT("CoreLabs:  utilities:  swap_bytes") )
{
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint16_t(0x1234) ), uint16_t(0x3412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint32_t(0x12345678) ), uint32_t(0x78563412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( uint64_t(0x123456789abcdef0) ), uint64_t(0xf0debc9a78563412) );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( -1 ), -1 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes( int32_t(0xff0000ff) ), 0xff0000ff );
#if ('AB') == 0x4142 // => little endian
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian( 0x12345678, std::endian::little ), 0x78563412 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian( 0x12345678, std::endian::big ), 0x12345678 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian( 0x12345678, std::endian::little ), 0x12345678 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian( 0x12345678, std::endian::big ), 0x78563412 );
#endif
#if ('AB') == 0x4241 // => big endian
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian( 0x12345678, std::endian::little ), 0x12345678 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_if_endian( 0x12345678, std::endian::big ), 0x78563412 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian( 0x12345678, std::endian::little ), 0x78563412 );
    TESTLABS_CHECK_EQ( cl7::bits::swap_bytes_unless_endian( 0x12345678, std::endian::big ), 0x12345678 );
#endif
}



#endif // F77_TESTS_CL7_UTILITIES_H
