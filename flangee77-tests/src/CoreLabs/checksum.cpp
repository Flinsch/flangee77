
#include <TestLabs/TestSuite.h>

#include <CoreLabs/checksum.h>
#include <CoreLabs/byte_vector.h>

#include "../shared.h"



TESTLABS_CASE( u8"CoreLabs:  checksum:  crc32" )
{
    TESTLABS_CHECK_EQ( cl7::checksum::crc32({}), 0x00000000u );

    // Standard CRC-32/ISO-HDLC check value (same one used by zlib/PNG/gzip)
    // for the ASCII string "123456789".
    const cl7::byte_vector check_data = cl7::make_bytes('1', '2', '3', '4', '5', '6', '7', '8', '9');
    TESTLABS_CHECK_EQ( cl7::checksum::crc32(check_data), 0xcbf43926u );

    const cl7::byte_vector other_data = cl7::make_bytes('1', '2', '3', '4', '5', '6', '7', '8', '0');
    TESTLABS_CHECK_NE( cl7::checksum::crc32(other_data), cl7::checksum::crc32(check_data) );
}
