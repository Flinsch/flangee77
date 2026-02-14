#ifndef F77_TESTS_XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H
#define F77_TESTS_XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H

#include <XiaoLabs/graphics/images/ImageProcessor.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  ImageProcessor" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 55 );


    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;
        xl7::graphics::ChannelOrder channel_order;
        cl7::byte_vector packed_data;
    } entry;

    const std::vector<Entry> container {
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x33 ) },
        { xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x33 ) },

        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x32 ) },

        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66, 0x33 ) },

        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33, 0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33,0x33, 0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32, 0x66,0x36 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x32, 0x66,0x36 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x36, 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x66,0x36, 0x66,0x32 ) },

        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x03 | 0x60, 0x09 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x03 | 0x60, 0x09 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x09 | 0x60, 0x03 ) },
        { xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x09 | 0x60, 0x03 ) },

        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10), (6<<0 | 12<<5 | 19<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10), (6<<0 | 12<<5 | 19<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10), (19<<0 | 12<<5 | 6<<10) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10), (19<<0 | 12<<5 | 6<<10) >> 8 ) },

        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 25<<5 | 19<<11), (6<<0 | 25<<5 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (6<<0 | 25<<5 | 19<<11), (6<<0 | 25<<5 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (19<<0 | 25<<5 | 6<<11), (19<<0 | 25<<5 | 6<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 25<<5 | 6<<11), (19<<0 | 25<<5 | 6<<11) >> 8 ) },

        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x99, 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33 ) },

        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33, 0x66, 0x99, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x99, 0x66, 0x33, 0x00 ) },
        { xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33, 0x00 ) },

        { xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33, 0x66,0x66, 0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x33,0x33, 0x66,0x66, 0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x99,0x99, 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99,0x99, 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16B16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32, 0x66,0x36, 0xcc,0x38 ) },
        { xl7::graphics::PixelFormat::R16G16B16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x32, 0x66,0x36, 0xcc,0x38 ) },
        { xl7::graphics::PixelFormat::R16G16B16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc,0x38, 0x66,0x36, 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16G16B16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcc,0x38, 0x66,0x36, 0x66,0x32 ) },

        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },

        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x03 | 0x60, 0x09 | 0xc0 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x0c | 0x30, 0x06 | 0x90 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x0c | 0x90, 0x06 | 0x30 ) },
        { xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x09 | 0x60, 0x03 | 0xc0 ) },

        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (6<<0 | 12<<5 | 19<<10 | 1<<15), (6<<0 | 12<<5 | 19<<10 | 1<<15) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (1<<0 | 6<<1 | 12<<6 | 19<<11), (1<<0 | 6<<1 | 12<<6 | 19<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (1<<0 | 19<<1 | 12<<6 | 6<<11), (1<<0 | 19<<1 | 12<<6 | 6<<11) >> 8 ) },
        { xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (19<<0 | 12<<5 | 6<<10 | 1<<15), (19<<0 | 12<<5 | 6<<10 | 1<<15) >> 8 ) },

        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33, 0x66, 0x99, 0xcc ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc, 0x33, 0x66, 0x99 ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc, 0x99, 0x66, 0x33 ) },
        { xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99, 0x66, 0x33, 0xcc ) },

        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( (204<<0 | 409<<10 | 614<<20 | 3<<30), (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 8, (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 16, (204<<0 | 409<<10 | 614<<20 | 3<<30) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( (3<<0 | 204<<2 | 409<<12 | 614<<22), (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 8, (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 16, (3<<0 | 204<<2 | 409<<12 | 614<<22) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( (3<<0 | 614<<2 | 409<<12 | 204<<22), (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 8, (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 16, (3<<0 | 614<<2 | 409<<12 | 204<<22) >> 24 ) },
        { xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( (614<<0 | 409<<10 | 204<<20 | 3<<30), (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 8, (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 16, (614<<0 | 409<<10 | 204<<20 | 3<<30) >> 24 ) },

        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x33,0x33, 0x66,0x66, 0x99,0x99, 0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc,0xcc, 0x33,0x33, 0x66,0x66, 0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc,0xcc, 0x99,0x99, 0x66,0x66, 0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x99,0x99, 0x66,0x66, 0x33,0x33, 0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x66,0x32, 0x66,0x36, 0xcc,0x38, 0x66,0x3a ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x66,0x3a, 0x66,0x32, 0x66,0x36, 0xcc,0x38 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x66,0x3a, 0xcc,0x38, 0x66,0x36, 0x66,0x32 ) },
        { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcc,0x38, 0x66,0x36, 0x66,0x32, 0x66,0x3a ) },

        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99, 0x00,0xcd,0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0x00,0xcd,0xcc,0xcc, 0x40,0x33,0x33,0x33, 0x80,0x66,0x66,0x66, 0x00,0x9a,0x99,0x99 ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0x00,0xcd,0xcc,0xcc, 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33 ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x00,0x9a,0x99,0x99, 0x80,0x66,0x66,0x66, 0x40,0x33,0x33,0x33, 0x00,0xcd,0xcc,0xcc ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0x4c,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3f, 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0xcc,0x3e, 0x9a,0x99,0x19,0x3f ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcd,0xcc,0x4c,0x3f, 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e ) },
        { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0x9a,0x99,0x19,0x3f, 0xcd,0xcc,0xcc,0x3e, 0xcd,0xcc,0x4c,0x3e, 0xcd,0xcc,0x4c,0x3f ) },

        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::ARGB, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::ABGR, cl7::make_bytes( 0xcc ) },
        { xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::BGRA, cl7::make_bytes( 0xcc ) },
    };

    const xl7::graphics::Color entry_color( 0.2f, 0.4f, 0.6f, 0.8f );


    auto _precision = [](unsigned depth) -> unsigned {
        if ( depth <= 2 )
            return 0;
        if ( depth <= 6 )
            return 1;
        if ( depth <= 10 )
            return 2;
        if ( depth <= 16 )
            return 3;
        // => depth > 16
        return     4;
    };


    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"pack_color/unpack_color", container, entry, cl7::to_string( entry.pixel_format ) + u8" " + cl7::to_string( entry.channel_order ) )
    {
        xl7::graphics::PixelLayout pixel_layout{ entry.pixel_format, entry.channel_order };
        size_t stride = static_cast<size_t>( pixel_layout.stride );

        cl7::byte_vector packed_data{ stride };
        xl7::graphics::images::ImageProcessor::pack_color( entry_color, entry.pixel_format, entry.channel_order, packed_data );
        xl7::graphics::Color color = xl7::graphics::images::ImageProcessor::unpack_color( entry.packed_data, entry.pixel_format, entry.channel_order );

        TESTLABS_CHECK_EQ( packed_data, entry.packed_data );

        if ( pixel_layout.uniform_depth == 8 && pixel_layout.channel_count == 4 ) // R8G8B8A8_UNORM
        {
            uint32_t packed_uint32 = *((const uint32_t*)(const uint8_t*)packed_data.data());
            xl7::graphics::Color color{ uint8_t(entry.packed_data[pixel_layout.r.index]), uint8_t(entry.packed_data[pixel_layout.g.index]), uint8_t(entry.packed_data[pixel_layout.b.index]), uint8_t(entry.packed_data[pixel_layout.a.index]) };
            TESTLABS_CHECK_EQ( packed_uint32, color.to_uint32( entry.channel_order ) );
        }

        for ( unsigned rgba = 0; rgba < 4; ++rgba )
        {
            unsigned depth = pixel_layout.channels[ rgba ].depth;
            if ( !depth )
                continue;

            unsigned prec = _precision( depth );

            TESTLABS_CHECK_EQ( ml7::round( color[rgba], prec ), ml7::round( entry_color[rgba], prec ) );
        }
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H
