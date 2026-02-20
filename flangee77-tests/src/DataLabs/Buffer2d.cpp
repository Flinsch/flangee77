
#include <TestLabs/TestSuite.h>

#include <DataLabs/Buffer2d.h>

#include "../shared.h"

#include <bit>
#include <vector>



TESTLABS_CASE( u8"DataLabs:  Buffer2d:  empty buffers" )
{
    std::vector<float> data;

    TESTLABS_CHECK_EQ( dl7::Buffer2d<float>().size(), 0 );
    TESTLABS_CHECK_EQ( dl7::Buffer2d<float>( data, 0, 0 ).size(), 0 );
}

TESTLABS_CASE( u8"DataLabs:  Buffer2d:  basic usage" )
{
    std::vector<float> data( 12 ); // 3x4 matrix
    for (size_t i = 0; i < 12; ++i)
        data[ i ] = i + 1;

    dl7::Buffer2d<float> buffer( data, 4, 3 );

    TESTLABS_CHECK_EQ( buffer.width(), 4 );
    TESTLABS_CHECK_EQ( buffer.height(), 3 );
    TESTLABS_CHECK_EQ( buffer.element_stride(), 4 );
    TESTLABS_CHECK_EQ( buffer.row_pitch(), 16 );
    TESTLABS_CHECK_EQ( buffer.size_bytes(), 48 );
    TESTLABS_CHECK_EQ( buffer.size(), 12 );
    TESTLABS_CHECK_EQ( buffer.element( 0, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 1 ), 6.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 2 ), 7.0f );
    TESTLABS_CHECK_EQ( buffer.element( 2, 3 ), 12.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0, 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 1 ), std::bit_cast<long>(6.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 2 ), std::bit_cast<long>(7.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 2, 3 ), std::bit_cast<long>(12.0f) );
}

TESTLABS_CASE( u8"DataLabs:  Buffer2d:  strided access" )
{
    std::vector<std::byte> data( 108 ); // 3x4 matrix, each element 4 bytes, stride 8, row pitch 40
    float* fdata = reinterpret_cast<float*>(data.data());
    for (size_t i = 0; i < 12; ++i)
    {
        size_t row = i / 4;
        size_t col = i % 4;
        size_t j = ((row * 5) + col) * 2;
        fdata[ j ] = i + 1;
    }

    dl7::Buffer2d<float> buffer( data, 4, 3, 8, 40 );

    TESTLABS_CHECK_EQ( buffer.width(), 4 );
    TESTLABS_CHECK_EQ( buffer.height(), 3 );
    TESTLABS_CHECK_EQ( buffer.element_stride(), 8 );
    TESTLABS_CHECK_EQ( buffer.row_pitch(), 40 );
    TESTLABS_CHECK_EQ( buffer.size_bytes(), 108 );
    TESTLABS_CHECK_EQ( buffer.size(), 12 );
    TESTLABS_CHECK_EQ( buffer.element( 0, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 1 ), 6.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 2 ), 7.0f );
    TESTLABS_CHECK_EQ( buffer.element( 2, 3 ), 12.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0, 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 1 ), std::bit_cast<long>(6.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 2 ), std::bit_cast<long>(7.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 2, 3 ), std::bit_cast<long>(12.0f) );
}

TESTLABS_CASE( u8"DataLabs:  Buffer2d:  row access" )
{
    std::vector<float> data( 12 ); // 3x4 matrix
    for (size_t i = 0; i < 12; ++i)
        data[ i ] = i + 1;

    dl7::Buffer2d<float> buffer( data, 4, 3 );

    TESTLABS_CHECK_EQ( buffer.row( 0 ).element( 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.row( 1 ).element( 1 ), 6.0f );
    TESTLABS_CHECK_EQ( buffer.row( 1 ).element( 2 ), 7.0f );
    TESTLABS_CHECK_EQ( buffer.row( 2 ).element( 3 ), 12.0f );
    TESTLABS_CHECK_EQ( buffer.row( 0 ).map_element<long>( 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.row( 1 ).map_element<long>( 1 ), std::bit_cast<long>(6.0f) );
    TESTLABS_CHECK_EQ( buffer.row( 1 ).map_element<long>( 2 ), std::bit_cast<long>(7.0f) );
    TESTLABS_CHECK_EQ( buffer.row( 2 ).map_element<long>( 3 ), std::bit_cast<long>(12.0f) );
}
