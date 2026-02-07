#ifndef F77_TESTS_CL7_IO_ASCIIWRITER_H
#define F77_TESTS_CL7_IO_ASCIIWRITER_H

#include <CoreLabs/io/AsciiWriter.h>
#include <CoreLabs/io/WritableMemory.h>
#include <CoreLabs/text/codec.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"CoreLabs:  io:  AsciiWriter" )
{
    cl7::io::WritableMemory writable_memory;
    cl7::io::AsciiWriter ascii_writer{ &writable_memory };

    TESTLABS_CHECK_EQ( ascii_writer.write_line( "Line 1", "\r\n" ), 6 );
    TESTLABS_CHECK_EQ( ascii_writer.write_line( "Line 2", "\n" ), 6 );
    TESTLABS_CHECK_EQ( ascii_writer.write_line( "Line 3", "\r" ), 6 );

    TESTLABS_CHECK_EQ( ascii_writer.write( "Hello World" ), 11 );
    TESTLABS_CHECK_EQ( ascii_writer.write_char( '!' ), 1 );

    TESTLABS_CHECK_EQ( cl7::to_bytes(writable_memory.get_data()), cl7::text::codec::to_bytes("Line 1\r\nLine 2\nLine 3\rHello World!") );
}



#endif // F77_TESTS_CL7_IO_ASCIIWRITER_H
