
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/codecs/targa/RleDecoder.h>
#include <XiaoLabs/graphics/images/codecs/targa/RleEncoder.h>

#include "CoreLabs/io/ReadableMemory.h"
#include "CoreLabs/io/WritableMemory.h"

#include "../../../../shared.h"



TESTLABS_CASE(u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE decoder decodes raw packet")
{
    cl7::byte_vector input = cl7::make_bytes(
        0x02, // => 3 raw pixels
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    );

    cl7::byte_vector expected = cl7::make_bytes(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    );

    cl7::io::ReadableMemory readable( input );
    cl7::byte_vector output( 9 );

    xl7::graphics::images::codecs::targa::RleDecoder decoder;
    auto result = decoder.decode( readable, 24, output );

    TESTLABS_CHECK( result == xl7::graphics::images::codecs::targa::RleDecoder::Result::Success );
    TESTLABS_CHECK_EQ( output, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE decoder decodes repeated pixels" )
{
    cl7::byte_vector input = cl7::make_bytes(
        0x82, // => 3 repeated pixels
        10, 20, 30
    );

    cl7::byte_vector expected = cl7::make_bytes(
        10, 20, 30,
        10, 20, 30,
        10, 20, 30
    );

    cl7::io::ReadableMemory readable( input );
    cl7::byte_vector output( 9 );

    xl7::graphics::images::codecs::targa::RleDecoder decoder;
    auto result = decoder.decode( readable, 24, output );

    TESTLABS_CHECK( result == xl7::graphics::images::codecs::targa::RleDecoder::Result::Success );
    TESTLABS_CHECK_EQ( output, expected );
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE encoder emits raw packet for unique pixels" )
{
    cl7::byte_vector input = cl7::make_bytes(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    );

    cl7::byte_vector expected = cl7::make_bytes(
        0x02, // => 3 raw pixels
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    );

    cl7::io::WritableMemory writable;

    xl7::graphics::images::codecs::targa::RleEncoder encoder;
    auto result = encoder.encode( writable, 24, input );

    cl7::byte_vector output;
    writable.swap_data( output );

    TESTLABS_CHECK( result == xl7::graphics::images::codecs::targa::RleEncoder::Result::Success );
    TESTLABS_CHECK_EQ( output, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE encoder emits repeated runs" )
{
    cl7::byte_vector input = cl7::make_bytes(
        10, 20, 30,
        10, 20, 30,
        10, 20, 30
    );

    cl7::byte_vector expected = cl7::make_bytes(
        0x82, // => 3 repeated pixels
        10, 20, 30
    );

    cl7::io::WritableMemory writable;

    xl7::graphics::images::codecs::targa::RleEncoder encoder;
    auto result = encoder.encode( writable, 24, input );

    cl7::byte_vector output;
    writable.swap_data( output );

    TESTLABS_CHECK( result == xl7::graphics::images::codecs::targa::RleEncoder::Result::Success );
    TESTLABS_CHECK_EQ( output, expected );
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE roundtrip encode decode" )
{
    cl7::byte_vector original = cl7::make_bytes(
        1, 1, 1,
        1, 1, 1,
        2, 2, 2,
        3, 3, 3,
        3, 3, 3,
        4, 4, 4,
        5, 5, 5,
        5, 5, 5,
        5, 5, 5
    );

    cl7::io::WritableMemory writable;

    xl7::graphics::images::codecs::targa::RleEncoder encoder;
    auto encoder_result = encoder.encode( writable, 24, original );

    cl7::byte_vector encoded;
    writable.swap_data( encoded );

    cl7::io::ReadableMemory readable( encoded );
    cl7::byte_vector decoded( original.size() );

    xl7::graphics::images::codecs::targa::RleDecoder decoder;
    auto decoder_result = decoder.decode( readable, 24, decoded );

    TESTLABS_CHECK( encoder_result == xl7::graphics::images::codecs::targa::RleEncoder::Result::Success );
    TESTLABS_CHECK( decoder_result == xl7::graphics::images::codecs::targa::RleDecoder::Result::Success );
    TESTLABS_CHECK_EQ( decoded, original );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE random roundtrip" )
{
    cl7::byte_vector original( 1365 * 3 );

    for ( auto& b : original )
        b = static_cast<std::byte>(::rand() % 8); // Intended to result in many/some repetitions for RLE.

    cl7::io::WritableMemory writable;

    xl7::graphics::images::codecs::targa::RleEncoder encoder;
    auto encoder_result = encoder.encode( writable, 24, original );

    cl7::byte_vector encoded;
    writable.swap_data( encoded );

    cl7::io::ReadableMemory readable( encoded );
    cl7::byte_vector decoded( original.size() );

    xl7::graphics::images::codecs::targa::RleDecoder decoder;
    auto decoder_result = decoder.decode( readable, 24, decoded );

    TESTLABS_CHECK( encoder_result == xl7::graphics::images::codecs::targa::RleEncoder::Result::Success );
    TESTLABS_CHECK( decoder_result == xl7::graphics::images::codecs::targa::RleDecoder::Result::Success );
    TESTLABS_CHECK_EQ( decoded, original );
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  RLE encoder splits runs at 128 pixels" )
{
    cl7::byte_vector input;
    input.reserve( 130 * 3 );

    for ( unsigned i = 0; i < 130; ++i )
    {
        input.push_back( std::byte{1} );
        input.push_back( std::byte{2} );
        input.push_back( std::byte{3} );
    }

    cl7::io::WritableMemory writable;

    xl7::graphics::images::codecs::targa::RleEncoder encoder;
    auto result = encoder.encode( writable, 24, input );

    cl7::byte_vector output;
    writable.swap_data( output );

    TESTLABS_CHECK( result == xl7::graphics::images::codecs::targa::RleEncoder::Result::Success );
    TESTLABS_CHECK_EQ( output.size(), 8 ); // 2 runs
    TESTLABS_CHECK_EQ( output[0], std::byte{0xff} ); // 128 repeated pixels
    TESTLABS_CHECK_EQ( output[4], std::byte{0x81} ); // 2 repated pixels
}
