
#include <TestLabs/TestSuite.h>

#include <CoreLabs/io/ByteReader.h>
#include <CoreLabs/io/ReadableMemory.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  io:  ByteReader" )
{
    std::vector<std::byte> data = cl7::make_bytes( 0x7f, 0x01, 0x02, 0x03, 0x04, 0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12, 0x01, 0x02, 0x03, 0x04 );

    cl7::io::ReadableMemory readable_memory{ data };
    cl7::io::ByteReader byte_reader{ &readable_memory };

    std::byte byte = byte_reader.peek_byte();
    TESTLABS_CHECK_EQ( byte, std::byte{0x7f} );
    TESTLABS_CHECK_EQ( byte_reader.peek_byte( byte ), 1 );
    TESTLABS_CHECK_EQ( byte, std::byte{0x7f} );
    TESTLABS_CHECK_EQ( byte_reader.read_byte(), std::byte{0x7f} );

    std::vector<std::byte> span( 4 );
    TESTLABS_CHECK_EQ( byte_reader.read_bytes( span ), 4 );
    TESTLABS_CHECK_EQ( span, cl7::make_bytes( 0x01, 0x02, 0x03, 0x04 ) );

    unsigned long temp = byte_reader.read_scalar<unsigned long, std::endian::big>();
    TESTLABS_CHECK_EQ( temp, 0x12345678ul );
    TESTLABS_CHECK_EQ( byte_reader.read_scalar<std::endian::little>( temp ), 4 );
    TESTLABS_CHECK_EQ( temp, 0x12345678ul );

    span = byte_reader.read_all();
    TESTLABS_CHECK_EQ( span, cl7::make_bytes( 0x01, 0x02, 0x03, 0x04 ) );
}
