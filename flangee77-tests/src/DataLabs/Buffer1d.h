#ifndef F77_TESTS_DL7_BUFFER1D_H
#define F77_TESTS_DL7_BUFFER1D_H

#include <DataLabs/Buffer1d.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"

#include <vector>



TESTLABS_CASE( u8"DataLabs:  Buffer1d:  zero-length buffers" )
{
    std::vector<float> data;

    TESTLABS_CHECK_EQ( dl7::Buffer1d<float>().length, 0 );
    TESTLABS_CHECK_EQ( dl7::Buffer1d<float>( data ).length, 0 );
}

TESTLABS_CASE( u8"DataLabs:  Buffer1d:  basic usage" )
{
    std::vector<float> data = { 1.0f, 2.0f, 3.0f, 4.0f };

    dl7::Buffer1d<float> buffer( data );

    TESTLABS_CHECK_EQ( buffer.length, 4 );
    TESTLABS_CHECK_EQ( buffer.element( 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element (1 ), 2.0f );
    TESTLABS_CHECK_EQ( buffer.element (2 ), 3.0f );
    TESTLABS_CHECK_EQ( buffer.element( 3 ), 4.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1 ), std::bit_cast<long>(2.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 2 ), std::bit_cast<long>(3.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 3 ), std::bit_cast<long>(4.0f) );
}

TESTLABS_CASE( u8"DataLabs:  Buffer1d:  strided access" )
{
    std::vector<std::byte> data( 28 ); // 4 elements, each 4 bytes, stride 8
    float* fdata = reinterpret_cast<float*>(data.data());
    fdata[ 0 ] = 1.0f;
    fdata[ 2 ] = 2.0f;
    fdata[ 4 ] = 3.0f;
    fdata[ 6 ] = 4.0f;

    dl7::Buffer1d<float> buffer( data, 4, 8 );

    TESTLABS_CHECK_EQ( buffer.length, 4 );
    TESTLABS_CHECK_EQ( buffer.element( 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1 ), 2.0f );
    TESTLABS_CHECK_EQ( buffer.element( 2 ), 3.0f );
    TESTLABS_CHECK_EQ( buffer.element( 3 ), 4.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1 ), std::bit_cast<long>(2.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 2 ), std::bit_cast<long>(3.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 3 ), std::bit_cast<long>(4.0f) );
}



#endif // F77_TESTS_DL7_BUFFER1D_H
