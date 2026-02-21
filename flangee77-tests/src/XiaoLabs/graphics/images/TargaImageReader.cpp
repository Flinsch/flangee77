
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/TargaImageReader.h>

#include <CoreLabs/platform/filesystem.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  TargaImageReader:  Grayscale 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-gray-8bit.tga" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0xff,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0xf8,
        0x7f, 0x7f, 0x7f, 0x82, 0xdc, 0x46, 0x91,
        0x82, 0xdc, 0x46, 0x82, 0xdc, 0x46, 0xe4
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageReader targa_image_reader;

        bool result = targa_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  TargaImageReader:  Pseudo-Truecolor 555x (15-bit)" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-555x.tga" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-555x-rle.tga" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00, 0x00,0x00, 0x00,0x00, 0xe7,0x1c, 0xe7,0x1c, 0xe7,0x1c, 0xff,0x7f,
        0x00,0x00, 0x00,0x00, 0x00,0x00, 0xe7,0x1c, 0xe7,0x1c, 0xe7,0x1c, 0xff,0x7f,
        0xef,0x3d, 0xef,0x3d, 0xef,0x3d, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xe0,0x7f,
        0xef,0x3d, 0xef,0x3d, 0xef,0x3d, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x1f,0x7c,
        0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xff,0x03
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageReader targa_image_reader;

        bool result = targa_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( (image.get_pixel_format() == xl7::graphics::PixelFormat::R5G5B5X1_UNORM) || (image.get_pixel_format() == xl7::graphics::PixelFormat::R5G5B5A1_UNORM) ); // R5G5B5A1 could happen.
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  TargaImageReader:  Pseudo-Truecolor with Alpha 5551 (16-bit)" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-alpha-5551.tga" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x80, 0x00,0x80, 0x00,0x80, 0xe7,0x9c, 0xe7,0x9c, 0xe7,0x9c, 0xff,0xff,
        0x00,0x80, 0x00,0x80, 0x00,0x80, 0xe7,0x9c, 0xe7,0x9c, 0xe7,0x9c, 0xff,0xff,
        0xef,0xbd, 0xef,0xbd, 0xef,0xbd, 0x00,0xfc, 0xe0,0x83, 0x1f,0x80, 0xe0,0x7f,
        0xef,0xbd, 0xef,0xbd, 0xef,0xbd, 0x00,0xfc, 0xe0,0x83, 0x1f,0x80, 0x1f,0x7c,
        0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0x00,0x7c, 0xe0,0x03, 0x1f,0x00, 0xff,0x03
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageReader targa_image_reader;

        bool result = targa_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R5G5B5A1_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  TargaImageReader:  Truecolor 8-bit (24-bit)" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-8bit.tga" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-8bit-rle.tga" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-8bit-indexed.tga" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0x00,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0xff,0x00,0xff,
        0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0x00,0x00,0xff, 0x00,0xff,0x00, 0xff,0x00,0x00, 0xff,0xff,0x00
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageReader targa_image_reader;

        bool result = targa_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  TargaImageReader:  Truecolor with Alpha 8-bit (32-bit)" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-alpha-8bit.tga" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/targa-rgb-alpha-8bit-rle.tga" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x00,0x00,0xff,0xff, 0x00,0xff,0x00,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0xff,0x00,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x00,0x00,0xff,0xff, 0x00,0xff,0x00,0xff, 0xff,0x00,0x00,0xff, 0xff,0x00,0xff,0x00,
        0x00,0x00,0xff,0x7f, 0x00,0xff,0x00,0x7f, 0xff,0x00,0x00,0x7f, 0x00,0x00,0xff,0x3f, 0x00,0xff,0x00,0x3f, 0xff,0x00,0x00,0x3f, 0xff,0xff,0x00,0x00
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::TargaImageReader targa_image_reader;

        bool result = targa_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::BGRA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}
