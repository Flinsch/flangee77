#ifndef F77_TESTS_CL7_IO_BYTEWRITER_H
#define F77_TESTS_CL7_IO_BYTEWRITER_H

#include <CoreLabs/io/ByteWriter.h>
#include <CoreLabs/io/WritableMemory.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  io:  ByteWriter" )
{
    cl7::io::WritableMemory writable_memory;
    cl7::io::ByteWriter byte_writer{ &writable_memory };

    TESTLABS_CHECK_EQ( byte_writer.write_byte( std::byte{0x7f} ), 1 );
    TESTLABS_CHECK_EQ( byte_writer.write_bytes( cl7::make_bytes( 0x01, 0x02, 0x03, 0x04 ) ), 4 );

    TESTLABS_CHECK_EQ( byte_writer.write_scalar<std::endian::big>( 0x12345678ul ), 4 );
    TESTLABS_CHECK_EQ( byte_writer.write_scalar<std::endian::little>( 0x12345678ul ), 4 );

    TESTLABS_CHECK_EQ( byte_writer.write_bytes( cl7::make_bytes( 0x01, 0x02, 0x03, 0x04 ) ), 4 );

    TESTLABS_CHECK_EQ( writable_memory.get_data(), cl7::make_bytes( 0x7f, 0x01, 0x02, 0x03, 0x04, 0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12, 0x01, 0x02, 0x03, 0x04 ) );
}



#endif // F77_TESTS_CL7_IO_BYTEWRITER_H
