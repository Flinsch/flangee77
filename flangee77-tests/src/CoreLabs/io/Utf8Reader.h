#ifndef F77_TESTS_CL7_IO_UTF8READER_H
#define F77_TESTS_CL7_IO_UTF8READER_H

#include <CoreLabs/io/Utf8Reader.h>
#include <CoreLabs/io/ReadableMemory.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"CoreLabs:  io:  Utf8Reader" )
{
    cl7::u8string string = u8"7ßFoo Bar Line 1\r\nLine 2\nLine 3\rHello World";

    cl7::io::ReadableMemory readable_memory{ cl7::make_byte_view(string) };
    cl7::io::Utf8Reader utf8_reader{ &readable_memory };

    TESTLABS_CHECK_EQ( utf8_reader.peek_char(), u8'7' );
    TESTLABS_CHECK_EQ( utf8_reader.read_char(), u8'7' );

    TESTLABS_CHECK_EQ( utf8_reader.peek_codepoint().length(), 2 );
    TESTLABS_CHECK_EQ( utf8_reader.peek_codepoint(), u8"ß" );
    TESTLABS_CHECK_EQ( utf8_reader.read_codepoint(), u8"ß" );

    TESTLABS_CHECK_EQ( utf8_reader.read_token(), u8"Foo" );
    TESTLABS_CHECK_EQ( utf8_reader.read_token_validated(), u8"Bar" );

    TESTLABS_CHECK_EQ( utf8_reader.read_char(), u8' ' );

    TESTLABS_CHECK_EQ( utf8_reader.read_line(), u8"Line 1" );
    TESTLABS_CHECK_EQ( utf8_reader.read_line_validated(), u8"Line 2" );
    TESTLABS_CHECK_EQ( utf8_reader.read_line(), u8"Line 3" );

    auto tmp = readable_memory.get_read_position();
    TESTLABS_CHECK_EQ( utf8_reader.read_all(), u8"Hello World" );
    readable_memory.set_read_position( tmp );
    TESTLABS_CHECK_EQ( utf8_reader.read_all_validated(), u8"Hello World" );
}



#endif // F77_TESTS_CL7_IO_UTF8READER_H
