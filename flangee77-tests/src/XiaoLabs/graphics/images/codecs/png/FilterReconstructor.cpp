
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/codecs/png/FilterReconstructor.h>

#include "../../../../shared.h"



namespace xl7::graphics::images::codecs::png {



// 3x2 image, 1 byte per pixel, bytes_per_scanline=3
// Raw pixel data: row0=[10,20,30], row1=[40,50,60]

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  filter None" )
{
    // Filter 0 (None): filtered bytes == raw bytes
    // src: [0, 10,20,30,  0, 40,50,60]
    const cl7::byte_vector src = cl7::make_bytes(
        0x00, 0x0a, 0x14, 0x1e,
        0x00, 0x28, 0x32, 0x3c
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e,
        0x28, 0x32, 0x3c
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  filter Sub" )
{
    // Filter 1 (Sub): filtered[col] = raw[col] - raw[col - bpp]
    // row0 filtered: [10, 20-10=10, 30-20=10]
    // row1 filtered: [40, 50-40=10, 60-50=10]
    // src: [1, 10,10,10,  1, 40,10,10]
    const cl7::byte_vector src = cl7::make_bytes(
        0x01, 0x0a, 0x0a, 0x0a,
        0x01, 0x28, 0x0a, 0x0a
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e,
        0x28, 0x32, 0x3c
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  filter Up" )
{
    // Filter 2 (Up): filtered[col] = raw[col] - raw_above[col]
    // row0 filtered: [10, 20, 30] (no row above, b=0)
    // row1 filtered: [40-10=30, 50-20=30, 60-30=30]
    // src: [2, 10,20,30,  2, 30,30,30]
    const cl7::byte_vector src = cl7::make_bytes(
        0x02, 0x0a, 0x14, 0x1e,
        0x02, 0x1e, 0x1e, 0x1e
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e,
        0x28, 0x32, 0x3c
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  filter Average" )
{
    // Filter 3 (Average): filtered[col] = raw[col] - floor((a+b)/2)
    // row0 (b=0): col0: 10-floor(0/2)=10, col1: 20-floor(10/2)=15, col2: 30-floor(20/2)=20
    // row1: col0: 40-floor((0+10)/2)=35, col1: 50-floor((40+20)/2)=20, col2: 60-floor((50+30)/2)=20
    // src: [3, 10,15,20,  3, 35,20,20]
    const cl7::byte_vector src = cl7::make_bytes(
        0x03, 0x0a, 0x0f, 0x14,
        0x03, 0x23, 0x14, 0x14
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e,
        0x28, 0x32, 0x3c
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  filter Paeth" )
{
    // Filter 4 (Paeth): filtered[col] = raw[col] - Paeth(a, b, c)
    // row0 (b=c=0): col0: P(0,0,0)=0 => 10, col1: P(10,0,0)=10 => 10, col2: P(20,0,0)=20 => 10
    // row1: col0: P(0,10,0)=10 => 40-10=30
    //        col1: P(40,20,10)=40 => 50-40=10
    //        col2: P(50,30,20)=50 => 60-50=10
    // src: [4, 10,10,10,  4, 30,10,10]
    const cl7::byte_vector src = cl7::make_bytes(
        0x04, 0x0a, 0x0a, 0x0a,
        0x04, 0x1e, 0x0a, 0x0a
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e,
        0x28, 0x32, 0x3c
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  RGB filter Sub" )
{
    // 2x1 image, 3 bytes per pixel (RGB), bytes_per_scanline=6
    // Raw: [R1,G1,B1=10,20,30, R2,G2,B2=40,60,80]
    // Filter Sub row0: [10,20,30, 40-10=30,60-20=40,80-30=50]
    // src: [1, 10,20,30, 30,40,50]
    const cl7::byte_vector src = cl7::make_bytes(
        0x01, 0x0a, 0x14, 0x1e, 0x1e, 0x28, 0x32
    );
    const cl7::byte_vector expected = cl7::make_bytes(
        0x0a, 0x14, 0x1e, 0x28, 0x3c, 0x50
    );

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 3, 6, 1 );

    TESTLABS_CHECK( result );
    TESTLABS_CHECK_EQ( dst, expected );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  FilterReconstructor:  reconstruct:  bad input size" )
{
    // Wrong src size => returns false
    const cl7::byte_vector src = cl7::make_bytes( 0x00, 0x0a, 0x14 ); // too short

    cl7::byte_vector dst;
    bool result = FilterReconstructor::reconstruct( src, dst, 1, 3, 2 );

    TESTLABS_CHECK( !result );
}



} // namespace xl7::graphics::images::codecs::png
