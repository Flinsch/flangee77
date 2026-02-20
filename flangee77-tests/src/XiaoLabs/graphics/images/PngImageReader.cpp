
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/PngImageReader.h>

#include <CoreLabs/platform/filesystem.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Grayscale 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-gray-8bit.png" },
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
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Grayscale with Alpha 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-gray-alpha-8bit.png" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x00,0xff, 0x00,0xff, 0x00,0xff, 0x3f,0xff, 0x3f,0xff, 0x3f,0xff, 0xff,0xff,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0xf8,0x00,
        0x7f,0xff, 0x7f,0xff, 0x7f,0xff, 0x82,0xff, 0xdc,0xff, 0x46,0xff, 0x91,0x00,
        0x82,0x7f, 0xdc,0x7f, 0x46,0x7f, 0x82,0x3f, 0xdc,0x3f, 0x46,0x3f, 0xe4,0x00
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Truecolor 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-rgb-8bit.png" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-rgb-8bit-indexed.png" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0x3f,0x3f,0x3f, 0xff,0xff,0xff,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0xff,0x00,
        0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0x7f,0x7f,0x7f, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0xff,
        0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0xff,0x00,0x00, 0x00,0xff,0x00, 0x00,0x00,0xff, 0x00,0xff,0xff
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Truecolor with Alpha 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-rgb-alpha-8bit.png" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x00,0x00,0x00,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0x3f,0x3f,0x3f,0xff, 0xff,0xff,0xff,0xff,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0xff,0x00,0x00,
        0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0x7f,0x7f,0x7f,0xff, 0xff,0x00,0x00,0xff, 0x00,0xff,0x00,0xff, 0x00,0x00,0xff,0xff, 0xff,0x00,0xff,0x00,
        0xff,0x00,0x00,0x7f, 0x00,0xff,0x00,0x7f, 0x00,0x00,0xff,0x7f, 0xff,0x00,0x00,0x3f, 0x00,0xff,0x00,0x3f, 0x00,0x00,0xff,0x3f, 0x00,0xff,0xff,0x00
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Grayscale 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-gray-16bit.png" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000, 0x0000, 0x0000, 0x3f3f, 0x3f3f, 0x3f3f, 0xffff,
        0x0000, 0x0000, 0x0000, 0x3f3f, 0x3f3f, 0x3f3f, 0xffff,
        0x7f7f, 0x7f7f, 0x7f7f, 0x8282, 0xdcdc, 0x4646, 0xf8f8,
        0x7f7f, 0x7f7f, 0x7f7f, 0x8282, 0xdcdc, 0x4646, 0x9191,
        0x8282, 0xdcdc, 0x4646, 0x8282, 0xdcdc, 0x4646, 0xe4e4
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Grayscale with Alpha 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-gray-alpha-16bit.png" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0xffff,0xffff,
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0x3f3f,0xffff, 0xffff,0xffff,
        0x7f7f,0xffff, 0x7f7f,0xffff, 0x7f7f,0xffff, 0x8282,0xffff, 0xdcdc,0xffff, 0x4646,0xffff, 0xf8f8,0x0000,
        0x7f7f,0xffff, 0x7f7f,0xffff, 0x7f7f,0xffff, 0x8282,0xffff, 0xdcdc,0xffff, 0x4646,0xffff, 0x9191,0x0000,
        0x8282,0x7f7f, 0xdcdc,0x7f7f, 0x4646,0x7f7f, 0x8282,0x3f3f, 0xdcdc,0x3f3f, 0x4646,0x3f3f, 0xe4e4,0x0000
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Truecolor 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-rgb-16bit.png" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0x3f3f,0x3f3f,0x3f3f, 0xffff,0xffff,0xffff,
        0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0xffff,0x0000,
        0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0x7f7f,0x7f7f,0x7f7f, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0xffff,
        0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0x0000,0xffff,0xffff
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  PngImageReader:  Truecolor with Alpha 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/png-rgb-alpha-16bit.png" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0x3f3f,0x3f3f,0x3f3f,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0xffff,0x0000,0x0000,
        0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0x7f7f,0x7f7f,0x7f7f,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0x0000,0xffff,0x0000,
        0xffff,0x0000,0x0000,0x7f7f, 0x0000,0xffff,0x0000,0x7f7f, 0x0000,0x0000,0xffff,0x7f7f, 0xffff,0x0000,0x0000,0x3f3f, 0x0000,0xffff,0x0000,0x3f3f, 0x0000,0x0000,0xffff,0x3f3f, 0x0000,0xffff,0xffff,0x0000
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::PngImageReader png_image_reader;

        bool result = png_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16A16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}
