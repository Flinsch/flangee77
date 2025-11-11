#ifndef F77_TESTS_CL7_MEMORY_H
#define F77_TESTS_CL7_MEMORY_H

#include <CoreLabs/memory.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  memory:  get_reasonable_byte_amount" )
{
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 0 ), 0 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1 ), 1 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 999 ), 999 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1000 ), 1000 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 9999 ), 9999 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 10000 ), 9 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 10000, 1000 ), 10 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1'000'000'000, 1000 ), 1000 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1'073'741'824, 1000 ), 1073 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1'073'741'824 ), 1024 );
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1'073'741'824, 1024, 1 ), 1'048'576 );
    unsigned c = 0;
    TESTLABS_CHECK_EQ( cl7::memory::get_reasonable_byte_amount( 1'073'741'824, 1024, -1, &c ), 1024 );
    TESTLABS_CHECK_EQ( c, 2 );
}

TESTLABS_CASE( u8"CoreLabs:  memory:  stringify_byte_amount_si" )
{
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 0 ), u8"0 MB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 0, u8"" ), u8"" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1 ), u8"1 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 999 ), u8"999 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1000 ), u8"1000 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 9999 ), u8"9999 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 10000 ), u8"10 kB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1'000'000'000 ), u8"1000 MB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1'073'741'824 ), u8"1073 MB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1'000'000'000'000 ), u8"1000 GB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1'000'000'000'000'000 ), u8"1000 TB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 1'000'000'000'000'000'000 ), u8"1000 PB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 10'000'000'000'000'000'000ull ), u8"10 EB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_si( 18'000'000'000'000'000'000ull ), u8"18 EB" );
}

TESTLABS_CASE( u8"CoreLabs:  memory:  stringify_byte_amount_iec" )
{
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 0 ), u8"0 MiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 0, u8"" ), u8"" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1 ), u8"1 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 999 ), u8"999 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1000 ), u8"1000 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 9999 ), u8"9999 B" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 10000 ), u8"9 KiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1'000'000'000 ), u8"953 MiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1'073'741'824 ), u8"1024 MiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1'000'000'000'000 ), u8"931 GiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1'000'000'000'000'000 ), u8"909 TiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 1'000'000'000'000'000'000 ), u8"888 PiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 10'000'000'000'000'000'000ull ), u8"8881 PiB" );
    TESTLABS_CHECK_EQ( cl7::memory::stringify_byte_amount_iec( 18'000'000'000'000'000'000ull ), u8"15 EiB" );
}



#endif // F77_TESTS_CL7_MEMORY_H
