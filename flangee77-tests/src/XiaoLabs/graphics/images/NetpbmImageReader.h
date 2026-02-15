#ifndef F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEREADER_H
#define F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEREADER_H

#include <XiaoLabs/graphics/images/NetpbmImageReader.h>

#include <CoreLabs/platform/filesystem.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  BLACKANDWHITE bitmap" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-ascii.pbm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-binary.pbm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-black-white.pam" },
    };

    const cl7::byte_vector data = cl7::make_bytes(
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00
    );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  GRAYSCALE 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-ascii-8bit.pgm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-binary-8bit.pgm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-gray-8bit.pam" },
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
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  GRAYSCALE_ALPHA 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-gray-alpha-8bit.pam" },
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
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  RGB 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-ascii-8bit.ppm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-binary-8bit.ppm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-rgb-8bit.pam" },
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
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  RGB_ALPHA 8-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-rgb-alpha-8bit.pam" },
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
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R8G8B8A8_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  GRAYSCALE 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-ascii-16bit.pgm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-binary-16bit.pgm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-gray-16bit.pam" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000, 0x0000, 0x0000, 0x3fff, 0x3fff, 0x3fff, 0xffff,
        0x0000, 0x0000, 0x0000, 0x3fff, 0x3fff, 0x3fff, 0xffff,
        0x7fff, 0x7fff, 0x7fff, 0x82ff, 0xdcff, 0x46ff, 0xf8ff,
        0x7fff, 0x7fff, 0x7fff, 0x82ff, 0xdcff, 0x46ff, 0x91ff,
        0x82ff, 0xdcff, 0x46ff, 0x82ff, 0xdcff, 0x46ff, 0xe4ff
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  GRAYSCALE_ALPHA 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-gray-alpha-16bit.pam" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0xffff,0xffff,
        0x0000,0xffff, 0x0000,0xffff, 0x0000,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0x3fff,0xffff, 0xffff,0xffff,
        0x7fff,0xffff, 0x7fff,0xffff, 0x7fff,0xffff, 0x82ff,0xffff, 0xdcff,0xffff, 0x46ff,0xffff, 0xf8ff,0x0000,
        0x7fff,0xffff, 0x7fff,0xffff, 0x7fff,0xffff, 0x82ff,0xffff, 0xdcff,0xffff, 0x46ff,0xffff, 0x91ff,0x0000,
        0x82ff,0x7fff, 0xdcff,0x7fff, 0x46ff,0x7fff, 0x82ff,0x3fff, 0xdcff,0x3fff, 0x46ff,0x3fff, 0xe4ff,0x0000
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}


TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  RGB 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-ascii-16bit.ppm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-binary-16bit.ppm" },
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-rgb-16bit.pam" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x0000,0x0000,0x0000, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0x3fff,0x3fff,0x3fff, 0xffff,0xffff,0xffff,
        0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0xffff,0x0000,
        0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0x7fff,0x7fff,0x7fff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0xffff,
        0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0xffff,0x0000,0x0000, 0x0000,0xffff,0x0000, 0x0000,0x0000,0xffff, 0x0000,0xffff,0xffff
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  NetpbmImageReader:  RGB_ALPHA 16-bit" )
{
    struct Entry
    {
        cl7::u8string file_path;
    } entry;

    const std::vector<Entry> container = {
        { cl7::platform::filesystem::get_working_directory() + u8"assets/XiaoLabs/graphics/images/netpbm-rgb-alpha-16bit.pam" },
    };

    const cl7::byte_vector data = cl7::to_bytes(cl7::make_byte_view(std::vector<uint16_t>{
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x0000,0x0000,0x0000,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0x3fff,0x3fff,0x3fff,0xffff, 0xffff,0xffff,0xffff,0xffff,
        0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0xffff,0x0000,0x0000,
        0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0x7fff,0x7fff,0x7fff,0xffff, 0xffff,0x0000,0x0000,0xffff, 0x0000,0xffff,0x0000,0xffff, 0x0000,0x0000,0xffff,0xffff, 0xffff,0x0000,0xffff,0x0000,
        0xffff,0x0000,0x0000,0x7fff, 0x0000,0xffff,0x0000,0x7fff, 0x0000,0x0000,0xffff,0x7fff, 0xffff,0x0000,0x0000,0x3fff, 0x0000,0xffff,0x0000,0x3fff, 0x0000,0x0000,0xffff,0x3fff, 0x0000,0xffff,0xffff,0x0000
    }));

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"load_from_file", container, entry, entry.file_path )
    {
        xl7::graphics::images::Image image;
        xl7::graphics::images::NetpbmImageReader netpbm_image_reader;

        bool result = netpbm_image_reader.load_from_file( entry.file_path, image );

        TESTLABS_CHECK( result );
        TESTLABS_CHECK_EQ( image.get_width(), 7 );
        TESTLABS_CHECK_EQ( image.get_height(), 5 );
        TESTLABS_CHECK_EQ( image.get_depth(), 1 );
        TESTLABS_CHECK( image.get_pixel_format() == xl7::graphics::PixelFormat::R16G16B16A16_UNORM );
        TESTLABS_CHECK( image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( cl7::to_bytes(image.get_data()), data );
    }
}



#endif // F77_TESTS_XL7_GRAPHICS_IMAGES_NETPBMIMAGEREADER_H
