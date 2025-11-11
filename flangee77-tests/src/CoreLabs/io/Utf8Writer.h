#ifndef F77_TESTS_CL7_IO_UTF8WRITER_H
#define F77_TESTS_CL7_IO_UTF8WRITER_H

#include <CoreLabs/io/Utf8Writer.h>
#include <CoreLabs/io/WritableMemory.h>
#include <CoreLabs/text/codec.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"CoreLabs:  io:  Utf8Writer" )
{
    cl7::io::WritableMemory writable_memory;
    cl7::io::Utf8Writer utf8_writer{ &writable_memory };

    TESTLABS_CHECK_EQ( utf8_writer.write_line( u8"Line 1", u8"\r\n" ), 6 );
    TESTLABS_CHECK_EQ( utf8_writer.write_line( u8"Line 2", u8"\n" ), 6 );
    TESTLABS_CHECK_EQ( utf8_writer.write_line( u8"Line 3", u8"\r" ), 6 );

    TESTLABS_CHECK_EQ( utf8_writer.write( u8"Hello World" ), 11 );

    TESTLABS_CHECK_EQ( writable_memory.get_data(), cl7::text::codec::to_bytes(u8"Line 1\r\nLine 2\nLine 3\rHello World") );
}



#endif // F77_TESTS_CL7_IO_UTF8WRITER_H
