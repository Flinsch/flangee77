
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/codecs/png/Writer.h>
#include <XiaoLabs/graphics/images/codecs/png/Reader.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>

#include <CoreLabs/platform/filesystem.h>

#include "../../../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Grayscale 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
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

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Grayscale with Alpha 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0xf8,0x00,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0x91,0x00,
        0x82,0x7f, 0xdc,0x7f, 0x46,0x7f, 0x82,0x3f, 0xdc,0x3f, 0x46,0x3f, 0xe4,0x00
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8G8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8G8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Truecolor 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0xff,0x00,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0xff,
        0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0x00,0xff,0xff
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8G8B8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Truecolor with Alpha 8-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0xff,0x00,0x00,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0x00,0xff,0x00,
        0xff,0x00,0x00,0x7f, 0x00,0xff,0x00,0x7f, 0x00,0x00,0xff,0x7f, 0xff,0x00,0x00,0x3f, 0x00,0xff,0x00,0x3f, 0x00,0x00,0xff,0x3f, 0x00,0xff,0xff,0x00
    );

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R8G8B8A8_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Grayscale 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000, 0x0000, 0x0000, 0x3f3f, 0x3f3f, 0x3f3f, 0xffff,
        0x0000, 0x0000, 0x0000, 0x3f3f, 0x3f3f, 0x3f3f, 0xffff,
        0x7f7f, 0x7f7f, 0x7f7f, 0x8282, 0xdcdc, 0x4646, 0xf8f8,
        0x7f7f, 0x7f7f, 0x7f7f, 0x8282, 0xdcdc, 0x4646, 0x9191,
        0x8282, 0xdcdc, 0x4646, 0x8282, 0xdcdc, 0x4646, 0xe4e4
    }));

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R16_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Grayscale with Alpha 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0xffff,0xffff,
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0xffff,0xffff,
        0x7f7f,0xffff, 0x7f7f,0xffff, 0x7f7f,0xffff, 0x8282,0xffff, 0xdcdc,0xffff, 0x4646,0xffff, 0xf8f8,0x0000,
        0x7f7f,0xffff, 0x7f7f,0xffff, 0x7f7f,0xffff, 0x8282,0xffff, 0xdcdc,0xffff, 0x4646,0xffff, 0x9191,0x0000,
        0x8282,0x7f7f, 0xdcdc,0x7f7f, 0x4646,0x7f7f, 0x8282,0x3f3f, 0xdcdc,0x3f3f, 0x4646,0x3f3f, 0xe4e4,0x0000
    }));

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R16G16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R16G16_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Truecolor 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0xffff,0xffff,0xffff,
        0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0xffff,0x0000,
        0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0xffff,
        0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0x0000,0xffff,0xffff
    }));

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R16G16B16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R16G16B16_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  Writer:  Truecolor with Alpha 16-bit" )
{
    struct Entry
    {
        xl7::graphics::ChannelOrder channel_order;
        xl7::graphics::images::codecs::png::Writer::Options writer_options;
        cl7::u8string string;
    } entry;

    std::vector<Entry> container;

    for ( auto filter_type = xl7::graphics::images::codecs::png::FilterEncoder::FilterType::None;
        filter_type <= xl7::graphics::images::codecs::png::FilterEncoder::FilterType::Auto;
        filter_type = static_cast<xl7::graphics::images::codecs::png::FilterEncoder::FilterType>(static_cast<int>(filter_type) + 1) )
    {
        for ( int compression_level = -1; compression_level <= 9; ++compression_level )
        {
            for ( unsigned k = 0; k < 4; ++k )
            {
                auto channel_order = static_cast<xl7::graphics::ChannelOrder>( k );
                auto string = cl7::to_string( channel_order ) + u8" " + cl7::to_string( static_cast<int>(filter_type) ) + u8" " + cl7::to_string( compression_level );
                container.push_back( { channel_order, { .filter_type = filter_type, .compression_level = compression_level }, string } );
            }
        }
    }

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0xffff,0x0000,0x0000,
        0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0x0000,0xffff,0x0000,
        0xffff,0x0000,0x0000,0x7f7f, 0x0000,0xffff,0x0000,0x7f7f, 0x0000,0x0000,0xffff,0x7f7f, 0xffff,0x0000,0x0000,0x3f3f, 0x0000,0xffff,0x0000,0x3f3f, 0x0000,0x0000,0xffff,0x3f3f, 0x0000,0xffff,0xffff,0x0000
    }));

    xl7::graphics::images::ImageDesc source_desc{ xl7::graphics::PixelFormat::R16G16B16A16_UNORM, xl7::graphics::ChannelOrder::RGBA, 7, 5, 1 };
    xl7::graphics::images::Image source_image{ source_desc, data };

    const auto file_path = cl7::platform::filesystem::get_temp_directory() + u8"png.tmp";

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"dump_to_file", container, entry, entry.string )
    {
        xl7::graphics::images::Image image = xl7::graphics::images::ImageConverter::convert_image( source_image, source_desc.pixel_format, entry.channel_order );
        xl7::graphics::images::codecs::png::Writer targa_image_writer{ entry.writer_options };
        xl7::graphics::images::codecs::png::Reader targa_image_reader;

        bool result = targa_image_writer.dump_to_file( image, file_path );
        image = {};
        result &= targa_image_reader.load_from_file( file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK_EQ( image.get_pixel_format(), xl7::graphics::PixelFormat::R16G16B16A16_UNORM );
        TESTLABS_CHECK_EQ( image.get_channel_order(), xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}
