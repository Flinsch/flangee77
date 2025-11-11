#ifndef F77_TESTS_CL7_IO_ASCIIREADER_H
#define F77_TESTS_CL7_IO_ASCIIREADER_H

#include <CoreLabs/io/AsciiReader.h>
#include <CoreLabs/io/ReadableMemory.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"CoreLabs:  io:  AsciiReader" )
{
    cl7::astring string = "7Foo Bar Line 1\r\nLine 2\nLine 3\rHello World";

    cl7::io::ReadableMemory readable_memory{ cl7::make_byte_view(string) };
    cl7::io::AsciiReader ascii_reader{ &readable_memory };

    TESTLABS_CHECK_EQ( ascii_reader.peek_char(), '7' );
    TESTLABS_CHECK_EQ( ascii_reader.read_char(), '7' );

    TESTLABS_CHECK_EQ( ascii_reader.read_token(), "Foo" );
    TESTLABS_CHECK_EQ( ascii_reader.read_token_validated(), "Bar" );

    TESTLABS_CHECK_EQ( ascii_reader.read_char(), ' ' );

    TESTLABS_CHECK_EQ( ascii_reader.read_line(), "Line 1" );
    TESTLABS_CHECK_EQ( ascii_reader.read_line_validated(), "Line 2" );
    TESTLABS_CHECK_EQ( ascii_reader.read_line(), "Line 3" );

    auto tmp = readable_memory.get_read_position();
    TESTLABS_CHECK_EQ( ascii_reader.read_all(), "Hello World" );
    readable_memory.set_read_position( tmp );
    TESTLABS_CHECK_EQ( ascii_reader.read_all_validated(), "Hello World" );
}



#endif // F77_TESTS_CL7_IO_ASCIIREADER_H
