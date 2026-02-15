#ifndef F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H
#define F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H

#include <XiaoLabs/graphics/images/NetpbmImageWriter.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>
#include <XiaoLabs/graphics/images/NetpbmImageReader.h>

#include <CoreLabs/platform/filesystem.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  BLACKANDWHITE bitmap" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 1, 4, 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00
    );

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  GRAYSCALE 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 2, 5, 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0xf8,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0x91,
        0x82, 0xdc, 0x46, 0x82, 0xdc, 0x46, 0xe4
    );

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  GRAYSCALE_ALPHA 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0xf8,0x00,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0x91,0x00,
        0x82,0x7f, 0xdc,0x7f, 0x46,0x7f, 0x82,0x3f, 0xdc,0x3f, 0x46,0x3f, 0xe4,0x00
    );

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  RGB 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 3, 6, 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0xff,0x00,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0xff,
        0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0x00,0xff,0xff
    );

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  RGB_ALPHA 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0xff,0x00,0x00,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0x00,0xff,0x00,
        0xff,0x00,0x00,0x7f, 0x00,0xff,0x00,0x7f, 0x00,0x00,0xff,0x7f, 0xff,0x00,0x00,0x3f, 0x00,0xff,0x00,0x3f, 0x00,0x00,0xff,0x3f, 0x00,0xff,0xff,0x00
    );

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  GRAYSCALE 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 2, 5, 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000, 0x0000, 0x0000, 0x3fff, 0x3fff, 0x3fff, 0xffff,
        0x0000, 0x0000, 0x0000, 0x3fff, 0x3fff, 0x3fff, 0xffff,
        0x7fff, 0x7fff, 0x7fff, 0x82ff, 0xdcff, 0x46ff, 0xf8ff,
        0x7fff, 0x7fff, 0x7fff, 0x82ff, 0xdcff, 0x46ff, 0x91ff,
        0x82ff, 0xdcff, 0x46ff, 0x82ff, 0xdcff, 0x46ff, 0xe4ff
    }));

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  GRAYSCALE_ALPHA 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0xffff,0xffff,
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0xffff,0xffff,
        0x7fff,0xffff, 0x7fff,0xffff, 0x7fff,0xffff, 0x82ff,0xffff, 0xdcff,0xffff, 0x46ff,0xffff, 0xf8ff,0x0000,
        0x7fff,0xffff, 0x7fff,0xffff, 0x7fff,0xffff, 0x82ff,0xffff, 0xdcff,0xffff, 0x46ff,0xffff, 0x91ff,0x0000,
        0x82ff,0x7fff, 0xdcff,0x7fff, 0x46ff,0x7fff, 0x82ff,0x3fff, 0xdcff,0x3fff, 0x46ff,0x3fff, 0xe4ff,0x0000
    }));

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  RGB 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 3, 6, 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0xffff,0xffff,0xffff,
        0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0xffff,0x0000,
        0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0xffff,
        0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0x0000,0xffff,0xffff
    }));

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageWriter:  RGB_ALPHA 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::NetpbmImageWriter::Format netpbm_format;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( unsigned p : std::vector<unsigned>{ 7 } )
    {
        for ( unsigned k = 0; k < 4; ++k )
        {
            auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
            auto netpbm_format = static_cast<xl7::graphics::images::NetpbmImageWriter::Format>( p );
            auto string = u8"P" + cl7::to_string( p ) + u8" " + cl7::to_string( channel_order );
            container.push_back( { channel_order, netpbm_format, string } );
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0xffff,0x0000,0x0000,
        0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0x0000,0xffff,0x0000,
        0xffff,0x0000,0x0000,0x7fff, 0x0000,0xffff,0x0000,0x7fff, 0x0000,0x0000,0xffff,0x7fff, 0xffff,0x0000,0x0000,0x3fff, 0x0000,0xffff,0x0000,0x3fff, 0x0000,0x0000,0xffff,0x3fff, 0x0000,0xffff,0xffff,0x0000
    }));

    xl7::graphics::images::Image::Desc source_desc{ xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"netpbm.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::NetpbmImageWriter netpbm_image_writer{ entry.netpbm_format };
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= netpbm_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16A16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H
