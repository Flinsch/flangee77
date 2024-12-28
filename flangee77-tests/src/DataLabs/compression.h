#pragma once
#ifndef F77_TESTS_DL7_COMPRESSION_H
#define F77_TESTS_DL7_COMPRESSION_H

#include <DataLabs/compression.h>

#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"DataLabs:  compression:  Deflate" )
{
    cl7::astring_view source_string = "This is just a simple test to simply check whether the end result is correct again.";

    cl7::byte_vector source_data = cl7::strings::to_bytes( source_string );
    cl7::byte_vector compressed_data;
    cl7::byte_vector result_data;

    dl7::compression::Deflate::compress( source_data, compressed_data );
    dl7::compression::Deflate::decompress( compressed_data, result_data );

    cl7::astring result_string = cl7::strings::to_ascii_unchecked( result_data );

    TESTLABS_CHECK_EQ( source_data, result_data );
    TESTLABS_CHECK_EQ( source_string, result_string );
}



#endif // F77_TESTS_DL7_COMPRESSION_H
