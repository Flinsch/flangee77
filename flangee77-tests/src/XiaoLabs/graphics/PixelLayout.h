#ifndef F77_TESTS_XL7_GRAPHICS_PIXELLAYOUT_H
#define F77_TESTS_XL7_GRAPHICS_PIXELLAYOUT_H

#include <XiaoLabs/graphics/PixelLayout.h>

#include <CoreLabs/text/format.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



inline bool operator==(const xl7::graphics::PixelLayout::Channel& lhs, const xl7::graphics::PixelLayout::Channel& rhs)
{
    return
        lhs.index == rhs.index &&
        lhs.depth == rhs.depth &&
        lhs.offset == rhs.offset &&
        lhs.mask == rhs.mask &&
        lhs.mask0 == rhs.mask0;
}



namespace tl7::internals {
    inline
    cl7::u8string to_string(const xl7::graphics::PixelLayout::DataType& data_type)
    {
        switch ( data_type )
        {
        case xl7::graphics::PixelLayout::DataType::UNKNOWN: return u8"#UNKNOWN";
        case xl7::graphics::PixelLayout::DataType::UNORM: return u8"#UNORM";
        case xl7::graphics::PixelLayout::DataType::SNORM: return u8"#SNORM";
        case xl7::graphics::PixelLayout::DataType::UINT: return u8"#UINT";
        case xl7::graphics::PixelLayout::DataType::SINT: return u8"#SINT";
        case xl7::graphics::PixelLayout::DataType::FLOAT: return u8"#FLOAT";
        default:
            assert( false );
        }
        return u8"#UNKNOWN";
    }

    inline
    cl7::u8string to_string(const xl7::graphics::PixelLayout::Channel& channel) { return u8"{" + cl7::to_string(channel.index) + u8", " + cl7::to_string(channel.depth) + u8", " + cl7::to_string(channel.offset) + u8", " + cl7::u8string(channel.mask ? cl7::text::format::to_0x_hex_lc(channel.mask) : u8"0") + u8", " + cl7::u8string(channel.mask0 ? cl7::text::format::to_0x_hex_lc(channel.mask0) : u8"0") + u8"}"; }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  PixelLayout" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 53 );


    TESTLABS_SUBCASE( u8"data type" )
    {
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UNORM );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_SNORM, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SNORM );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R10G10B10A2_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::UINT );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_SINT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::SINT );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R11G11B10_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).data_type, xl7::graphics::PixelLayout::DataType::FLOAT );
    }


    TESTLABS_SUBCASE( u8"channel count" )
    {
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 2 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R4G4B4X4_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G6B5_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8X8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R11G11B10_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 3 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R4G4B4A4_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R8G8B8A8_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R10G10B10A2_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R10G10B10A2_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_SNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R16G16B16A16_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );

        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_UINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_SINT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );
        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::R32G32B32A32_FLOAT, xl7::graphics::ChannelOrder::RGBA ).channel_count, 4 );


        TESTLABS_CHECK_EQ( xl7::graphics::PixelLayout( xl7::graphics::PixelFormat::A8_UNORM, xl7::graphics::ChannelOrder::RGBA ).channel_count, 1 );
    }


    {
        struct Entry
        {
            xl7::graphics::PixelFormat pixel_format;
            unsigned stride;
            xl7::graphics::PixelLayout::Channel r, g, b, a;
        };

        const std::vector<Entry> container {
            { xl7::graphics::PixelFormat::R8_UNORM, 1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_SNORM, 1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_UINT,  1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R8_SINT,  1, { 0, 8, 0, 0xff, 0xff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R16_UNORM,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_SNORM,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_UINT,     2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_SINT,     2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R16_FLOAT,    2, { 0, 16, 0, 0xffff, 0xffff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R32_UINT,     4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R32_SINT,     4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },
            { xl7::graphics::PixelFormat::R32_FLOAT,    4, { 0, 32, 0, 0xffffffff, 0xffffffff }, {}, {}, {} },

            { xl7::graphics::PixelFormat::R8G8_UNORM,   2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_SNORM,   2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_UINT,    2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },
            { xl7::graphics::PixelFormat::R8G8_SINT,    2, { 0, 8, 0, 0x00ff, 0x00ff }, { 1, 8, 8, 0xff00, 0x00ff }, {}, {} },

            { xl7::graphics::PixelFormat::R16G16_UNORM, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_SNORM, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_UINT,  4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_SINT,  4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },
            { xl7::graphics::PixelFormat::R16G16_FLOAT, 4, { 0, 16, 0, 0x0000ffff, 0x0000ffff }, { 1, 16, 16, 0xffff0000, 0x0000ffff }, {}, {} },

            { xl7::graphics::PixelFormat::R32G32_UINT,  8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },
            { xl7::graphics::PixelFormat::R32G32_SINT,  8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },
            { xl7::graphics::PixelFormat::R32G32_FLOAT, 8, { 0, 32, 0, 0x00000000ffffffff, 0x00000000ffffffff }, { 1, 32, 32, 0xffffffff00000000, 0x00000000ffffffff }, {}, {} },

            { xl7::graphics::PixelFormat::R4G4B4X4_UNORM,   2, { 0, 4, 0, 0x000f, 0x000f }, { 1, 4, 4, 0x00f0, 0x000f }, { 2, 4,  8, 0x0f00, 0x000f }, {} },
            { xl7::graphics::PixelFormat::R5G5B5X1_UNORM,   2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 5, 5, 0x03e0, 0x001f }, { 2, 5, 10, 0x7c00, 0x001f }, {} },
            { xl7::graphics::PixelFormat::R5G6B5_UNORM,     2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 6, 5, 0x07e0, 0x003f }, { 2, 5, 11, 0xf800, 0x001f }, {} },

            { xl7::graphics::PixelFormat::R8G8B8_UNORM, 3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_SNORM, 3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_UINT,  3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },
            { xl7::graphics::PixelFormat::R8G8B8_SINT,  3, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },

            { xl7::graphics::PixelFormat::R8G8B8X8_UNORM,   4, { 0, 8, 0, 0x0000ff, 0x0000ff }, { 1, 8, 8, 0x00ff00, 0x0000ff }, { 2, 8, 16, 0xff0000, 0x0000ff }, {} },

            { xl7::graphics::PixelFormat::R11G11B10_FLOAT,  4, { 0, 11, 0, 0x000007ff, 0x000007ff }, { 1, 11, 11, 0x003ff800, 0x000007ff }, { 2, 10, 22, 0xffc00000, 0x000003ff }, {} },

            { xl7::graphics::PixelFormat::R32G32B32_UINT,   12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },
            { xl7::graphics::PixelFormat::R32G32B32_SINT,   12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },
            { xl7::graphics::PixelFormat::R32G32B32_FLOAT,  12, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, {} },

            { xl7::graphics::PixelFormat::R4G4B4A4_UNORM,   2, { 0, 4, 0, 0x000f, 0x000f }, { 1, 4, 4, 0x00f0, 0x000f }, { 2, 4,  8, 0x0f00, 0x000f }, { 3, 4, 12, 0xf000, 0x000f } },
            { xl7::graphics::PixelFormat::R5G5B5A1_UNORM,   2, { 0, 5, 0, 0x001f, 0x001f }, { 1, 5, 5, 0x03e0, 0x001f }, { 2, 5, 10, 0x7c00, 0x001f }, { 3, 1, 15, 0x8000, 0x0001 } },

            { xl7::graphics::PixelFormat::R8G8B8A8_UNORM,   4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_SNORM,   4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_UINT,    4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },
            { xl7::graphics::PixelFormat::R8G8B8A8_SINT,    4, { 0, 8, 0, 0x000000ff, 0x000000ff }, { 1, 8, 8, 0x0000ff00, 0x000000ff }, { 2, 8, 16, 0x00ff0000, 0x000000ff }, { 3, 8, 24, 0xff000000, 0x000000ff } },

            { xl7::graphics::PixelFormat::R10G10B10A2_UNORM,    4, { 0, 10, 0, 0x000003ff, 0x000003ff }, { 1, 10, 10, 0x000ffc00, 0x000003ff }, { 2, 10, 20, 0x3ff00000, 0x000003ff }, { 3, 2, 30, 0xc0000000, 0x00000003 } },
            { xl7::graphics::PixelFormat::R10G10B10A2_UINT,     4, { 0, 10, 0, 0x000003ff, 0x000003ff }, { 1, 10, 10, 0x000ffc00, 0x000003ff }, { 2, 10, 20, 0x3ff00000, 0x000003ff }, { 3, 2, 30, 0xc0000000, 0x00000003 } },

            { xl7::graphics::PixelFormat::R16G16B16A16_UNORM,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_SNORM,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_UINT,    8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_SINT,    8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },
            { xl7::graphics::PixelFormat::R16G16B16A16_FLOAT,   8, { 0, 16, 0, 0x000000000000ffff, 0x000000000000ffff }, { 1, 16, 16, 0x00000000ffff0000, 0x000000000000ffff }, { 2, 16, 32, 0x0000ffff00000000, 0x000000000000ffff }, { 3, 16, 48, 0xffff000000000000, 0x000000000000ffff } },

            { xl7::graphics::PixelFormat::R32G32B32A32_UINT,    16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },
            { xl7::graphics::PixelFormat::R32G32B32A32_SINT,    16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },
            { xl7::graphics::PixelFormat::R32G32B32A32_FLOAT,   16, { 0, 32, 0, 0, 0xffffffff }, { 1, 32, 32, 0, 0xffffffff }, { 2, 32, 64, 0, 0xffffffff }, { 3, 32, 96, 0, 0xffffffff } },

            { xl7::graphics::PixelFormat::A8_UNORM, 1, {}, {}, {}, { 0, 8, 0, 0xff, 0xff } },
        };

        for ( size_t i = 0; i < container.size(); ++i )
        {
            const Entry& entry = container[ i ];

            for ( unsigned j = 0; j < 4; ++j )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>(j);

                xl7::graphics::PixelLayout pixel_layout{ entry.pixel_format, channel_order };

                TESTLABS_CHECK_EQ( pixel_layout.stride, entry.stride );

                if ( channel_order == xl7::graphics::ChannelOrder::RGBA )
                {
                    TESTLABS_CHECK_EQ( pixel_layout.r, entry.r );
                    TESTLABS_CHECK_EQ( pixel_layout.g, entry.g );
                    TESTLABS_CHECK_EQ( pixel_layout.b, entry.b );
                    TESTLABS_CHECK_EQ( pixel_layout.a, entry.a );
                }

                for ( unsigned k = 0; k < 4; ++k )
                {
                    const xl7::graphics::PixelLayout::Channel& entry_channel = *((&entry.r) + k);
                    TESTLABS_CHECK_EQ( pixel_layout.channels[k].depth, entry_channel.depth );
                    TESTLABS_CHECK_EQ( pixel_layout.channels[k].mask0, entry_channel.mask0 );
                }

                for ( unsigned k = 0; k < pixel_layout.channel_count; ++k )
                {
                    TESTLABS_CHECK_EQ( pixel_layout.channels[pixel_layout.logical_to_semantic[k]].index, k );
                }

                unsigned channel_index = 0;

                for ( unsigned k = 0; k < 4; ++k )
                {
                    if ( pixel_layout.channels[k].depth == 0 )
                        continue;

                    TESTLABS_CHECK_EQ( pixel_layout.semantic_involved[channel_index], k );

                    ++channel_index;
                }

                TESTLABS_CHECK_EQ( channel_index, pixel_layout.channel_count );
            } // for each channel order
        } // for each entry
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_PIXELLAYOUT_H
