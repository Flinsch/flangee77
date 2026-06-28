
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/codecs/targa/Writer.h>
#include <XiaoLabs/graphics/images/codecs/targa/Reader.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>

#include <CoreLabs/platform/filesystem.h>

#include "../../../../shared.h"



namespace cl7 {
    inline
    cl7::u8string to_string(const xl7::graphics::images::codecs::targa::Writer::Origin& origin)
    {
        switch ( origin )
        {
        case xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft: return u8"#BottomLeft";
        case xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft: return u8"#TopLeft";
        default:
            assert( false );
        }
        return u8"#Unknown";
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  Writer:  Grayscale 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        bool compression;
        xl7::graphics::images::codecs::targa::Writer::Origin origin;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    constexpr xl7::graphics::images::codecs::targa::Writer::Origin origins[ 2 ] = {
        xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft,
        xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft,
    };

    for ( bool compression : std::vector<bool>{ false, true } )
    {
        for ( auto origin : origins )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + (compression ? u8"compressed" : u8"uncompressed") + u8" " + cl7::to_string( origin );
                container.push_back( { channel_order, compression, origin, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0xf8,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0x91,
        0x82, 0xdc, 0x46, 0x82, 0xdc, 0x46, 0xe4
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"targa.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::targa::Writer targa_image_writer{ { .compression = entry.compression, .origin = entry.origin } };
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  Writer:  Pseudo-Truecolor 555x (15-bit)" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        bool compression;
        xl7::graphics::images::codecs::targa::Writer::Origin origin;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    constexpr xl7::graphics::images::codecs::targa::Writer::Origin origins[ 2 ] = {
        xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft,
        xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft,
    };

    for ( bool compression : std::vector<bool>{ false, true } )
    {
        for ( auto origin : origins )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + (compression ? u8"compressed" : u8"uncompressed") + u8" " + cl7::to_string( origin );
                container.push_back( { channel_order, compression, origin, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00, 0x00,0x00, 0x00,0x00, 0xe7,0x1c, 0xe7,0x1c, 0xe7,0x1c, 0xff,0x7f,
        0x00,0x00, 0x00,0x00, 0x00,0x00, 0xe7,0x1c, 0xe7,0x1c, 0xe7,0x1c, 0xff,0x7f,
        0xef,0x3d, 0xef,0x3d, 0xef,0x3d, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xe0,0x7f,
        0xef,0x3d, 0xef,0x3d, 0xef,0x3d, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x1f,0x7c,
        0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xff,0x03
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R5G5B5X1_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"targa.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::targa::Writer targa_image_writer{ { .compression = entry.compression, .origin = entry.origin } };
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R5G5B5X1_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  Writer:  Pseudo-Truecolor with Alpha 5551 (16-bit)" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        bool compression;
        xl7::graphics::images::codecs::targa::Writer::Origin origin;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    constexpr xl7::graphics::images::codecs::targa::Writer::Origin origins[ 2 ] = {
        xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft,
        xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft,
    };

    for ( bool compression : std::vector<bool>{ false, true } )
    {
        for ( auto origin : origins )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + (compression ? u8"compressed" : u8"uncompressed") + u8" " + cl7::to_string( origin );
                container.push_back( { channel_order, compression, origin, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x80, 0x00,0x80, 0x00,0x80, 0xe7,0x9c, 0xe7,0x9c, 0xe7,0x9c, 0xff,0xff,
        0x00,0x80, 0x00,0x80, 0x00,0x80, 0xe7,0x9c, 0xe7,0x9c, 0xe7,0x9c, 0xff,0xff,
        0xef,0xbd, 0xef,0xbd, 0xef,0xbd, 0x00,0xfc, 0xe0,0x83, 0x1f,0x80, 0xe0,0x7f,
        0xef,0xbd, 0xef,0xbd, 0xef,0xbd, 0x00,0xfc, 0xe0,0x83, 0x1f,0x80, 0x1f,0x7c,
        0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xff,0x03
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R5G5B5A1_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"targa.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::targa::Writer targa_image_writer{ { .compression = entry.compression, .origin = entry.origin } };
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R5G5B5A1_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  Writer:  Truecolor 8-bit (24-bit)" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        bool compression;
        xl7::graphics::images::codecs::targa::Writer::Origin origin;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    constexpr xl7::graphics::images::codecs::targa::Writer::Origin origins[ 2 ] = {
        xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft,
        xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft,
    };

    for ( bool compression : std::vector<bool>{ false, true } )
    {
        for ( auto origin : origins )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + (compression ? u8"compressed" : u8"uncompressed") + u8" " + cl7::to_string( origin );
                container.push_back( { channel_order, compression, origin, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0x00,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0xff,0x00,0xff,
        0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0xff,0xff,0x00
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"targa.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::targa::Writer targa_image_writer{ { .compression = entry.compression, .origin = entry.origin } };
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  targa:  Writer:  Truecolor with Alpha 8-bit (32-bit)" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        bool compression;
        xl7::graphics::images::codecs::targa::Writer::Origin origin;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    constexpr xl7::graphics::images::codecs::targa::Writer::Origin origins[ 2 ] = {
        xl7::graphics::images::codecs::targa::Writer::Origin::BottomLeft,
        xl7::graphics::images::codecs::targa::Writer::Origin::TopLeft,
    };

    for ( bool compression : std::vector<bool>{ false, true } )
    {
        for ( auto origin : origins )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + (compression ? u8"compressed" : u8"uncompressed") + u8" " + cl7::to_string( origin );
                container.push_back( { channel_order, compression, origin, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x00,0x00,0xff,0xff, 0x00,0xff,0x00,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0xff,0x00,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x00,0x00,0xff,0xff, 0x00,0xff,0x00,0xff, 0xff,0x00,0x00,0xff, 0xff,0x00,0xff,0x00,
        0x00,0x00,0xff,0x7f, 0x00,0xff,0x00,0x7f, 0xff,0x00,0x00,0x7f, 0x00,0x00,0xff,0x3f, 0x00,0xff,0x00,0x3f, 0xff,0x00,0x00,0x3f, 0xff,0xff,0x00,0x00
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"targa.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::targa::Writer targa_image_writer{ { .compression = entry.compression, .origin = entry.origin } };
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}
