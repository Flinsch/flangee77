
#include <TestLabs/TestSuite.h>

#include <DataLabs/Buffer3d.h>

#include "../shared.h"

#include <bit>
#include <vector>



TESTLABS_CASE( u8"DataLabs:  Buffer3d:  empty buffers" )
{
    std::vector<float> data;

    TESTLABS_CHECK_EQ( dl7::Buffer3d<float>().size(), 0 );
    TESTLABS_CHECK_EQ( dl7::Buffer3d<float>( data, 0, 0, 0 ).size(), 0 );
}

TESTLABS_CASE( u8"DataLabs:  Buffer3d:  basic usage" )
{
    std::vector<float> data( 24 ); // 4x3x2 3D buffer
    for (size_t i = 0; i < 24; ++i)
        data[ i ] = i + 1;

    dl7::Buffer3d<float> buffer( data, 4, 3, 2 );

    TESTLABS_CHECK_EQ( buffer.width(), 4 );
    TESTLABS_CHECK_EQ( buffer.height(), 3 );
    TESTLABS_CHECK_EQ( buffer.depth(), 2 );
    TESTLABS_CHECK_EQ( buffer.element_stride(), 4 );
    TESTLABS_CHECK_EQ( buffer.row_pitch(), 16 );
    TESTLABS_CHECK_EQ( buffer.slice_pitch(), 48 );
    TESTLABS_CHECK_EQ( buffer.size_bytes(), 96 );
    TESTLABS_CHECK_EQ( buffer.size(), 24 );
    TESTLABS_CHECK_EQ( buffer.element( 0, 0, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 2, 3 ), 24.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0, 0, 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 2, 3 ), std::bit_cast<long>(24.0f) );
}

TESTLABS_CASE( u8"DataLabs:  Buffer3d:  strided access" )
{
    std::vector<std::byte> data( 236 ); // 4x3x2 3D buffer, each element 4 bytes, stride 8, row pitch 40, slice pitch 128
    float* fdata = reinterpret_cast<float*>(data.data());
    for (size_t i = 0; i < 24; ++i)
    {
        size_t slice = i / 12;
        size_t rem = i % 12;
        size_t row = rem / 4;
        size_t col = rem % 4;
        size_t j = ((slice * 16) + (row * 5) + col) * 2;
        fdata[ j ] = i + 1;
    }

    dl7::Buffer3d<float> buffer( data, 4, 3, 2, 8, 40, 128 );

    TESTLABS_CHECK_EQ( buffer.width(), 4 );
    TESTLABS_CHECK_EQ( buffer.height(), 3 );
    TESTLABS_CHECK_EQ( buffer.depth(), 2 );
    TESTLABS_CHECK_EQ( buffer.element_stride(), 8 );
    TESTLABS_CHECK_EQ( buffer.row_pitch(), 40 );
    TESTLABS_CHECK_EQ( buffer.slice_pitch(), 128 );
    TESTLABS_CHECK_EQ( buffer.size_bytes(), 236 );
    TESTLABS_CHECK_EQ( buffer.size(), 24 );
    TESTLABS_CHECK_EQ( buffer.element( 0, 0, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.element( 1, 2, 3 ), 24.0f );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 0, 0, 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.map_element<long>( 1, 2, 3 ), std::bit_cast<long>(24.0f) );
}

TESTLABS_CASE( u8"DataLabs:  Buffer3d:  slice access" )
{
    std::vector<float> data( 24 ); // 4x3x2 3D buffer
    for (size_t i = 0; i < 24; ++i)
        data[ i ] = i + 1;

    dl7::Buffer3d<float> buffer( data, 4, 3, 2 );

    TESTLABS_CHECK_EQ( buffer.slice( 0 ).element( 0, 0 ), 1.0f );
    TESTLABS_CHECK_EQ( buffer.slice( 1 ).element( 2, 3 ), 24.0f );
    TESTLABS_CHECK_EQ( buffer.slice( 0 ).map_element<long>( 0, 0 ), std::bit_cast<long>(1.0f) );
    TESTLABS_CHECK_EQ( buffer.slice( 1 ).map_element<long>( 2, 3 ), std::bit_cast<long>(24.0f) );
}
