
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/ImageResizer.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  ImageResizer" )
{
    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;

        unsigned source_width, source_height, source_depth;
        unsigned target_width, target_height, target_depth;

        xl7::graphics::images::ResamplingMethod resampling_method;

        cl7::byte_vector source_data;
        cl7::byte_vector target_data;
    } entry;

    const std::vector<Entry> container = {
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 0x7f ),  cl7::make_bytes( 0x7f ) },


        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 14 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  3, 1, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 13,14,15 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 3, 1,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 11,14,17 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 3, 3,  1, 1, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 1,2,3, 4,5,6, 7,8,9,  10,11,12, 13,14,15, 16,17,18,  19,20,21, 22,23,24, 25,26,27 ),  cl7::make_bytes( 5,14,23 ) },

        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 14 ),        cl7::make_bytes( 14,14,14, 14,14,14, 14,14,14,  14,14,14, 14,14,14, 14,14,14,  14,14,14, 14,14,14, 14,14,14 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  3, 1, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 13,14,15 ),  cl7::make_bytes( 13,14,15, 13,14,15, 13,14,15,  13,14,15, 13,14,15, 13,14,15,  13,14,15, 13,14,15, 13,14,15 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 3, 1,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 11,14,17 ),  cl7::make_bytes( 11,11,11, 14,14,14, 17,17,17,  11,11,11, 14,14,14, 17,17,17,  11,11,11, 14,14,14, 17,17,17 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  1, 1, 3,  3, 3, 3,  xl7::graphics::images::ResamplingMethod::NearestNeighbor,  cl7::make_bytes( 5,14,23 ),   cl7::make_bytes( 5,5,5, 5,5,5, 5,5,5,  14,14,14, 14,14,14, 14,14,14,  23,23,23, 23,23,23, 23,23,23 ) },


        { xl7::graphics::PixelFormat::R8_UNORM,  4, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2,3,4 ),  cl7::make_bytes( 2,4 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  2, 2, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2, 3,4 ),  cl7::make_bytes( 3 ) },
        { xl7::graphics::PixelFormat::R8_UNORM,  2, 2, 2,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 1,2, 3,4,  5,6, 7,8 ),  cl7::make_bytes( 5 ) },


        { xl7::graphics::PixelFormat::R32_FLOAT,  1, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  1, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  3, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  3, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40 ),  cl7::make_bytes( 0xab,0xaa,0xaa,0x3f, 0xab,0xaa,0x2a,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  4, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  4, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40 ),  cl7::make_bytes( 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x60,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 1, 1,  5, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x00,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  5, 1, 1,  2, 1, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40, 0x00,0x00,0xa0,0x40 ),  cl7::make_bytes( 0x66,0x66,0xe6,0x3f, 0x66,0x66,0x86,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  2, 2, 1,  3, 3, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40,  0x00,0x00,0x40,0x40, 0x00,0x00,0x80,0x40 ),  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0xc0,0x3f, 0x00,0x00,0x00,0x40,  0x00,0x00,0x00,0x40, 0x00,0x00,0x20,0x40, 0x00,0x00,0x40,0x40,  0x00,0x00,0x40,0x40, 0x00,0x00,0x60,0x40, 0x00,0x00,0x80,0x40 ) },
        { xl7::graphics::PixelFormat::R32_FLOAT,  3, 3, 1,  2, 2, 1,  xl7::graphics::images::ResamplingMethod::LinearInterpolation,  cl7::make_bytes( 0x00,0x00,0x80,0x3f, 0x00,0x00,0x00,0x40, 0x00,0x00,0x40,0x40,  0x00,0x00,0x80,0x40, 0x00,0x00,0xa0,0x40, 0x00,0x00,0xc0,0x40,  0x00,0x00,0xe0,0x40, 0x00,0x00,0x00,0x41, 0x00,0x00,0x10,0x41 ),  cl7::make_bytes( 0x55,0x55,0x15,0x40, 0xab,0xaa,0x6a,0x40,  0xab,0xaa,0xca,0x40, 0x55,0x55,0xf5,0x40 ) },
    };


    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"resize_image", container, entry, cl7::to_string( entry.pixel_format ) + u8" " + cl7::to_string(entry.source_width) + u8"x" + cl7::to_string(entry.source_height) + u8"x" + cl7::to_string(entry.source_depth) + u8" -> " + cl7::to_string(entry.target_width) + u8"x" + cl7::to_string(entry.target_height) + u8"x" + cl7::to_string(entry.target_depth) )
    {
        xl7::graphics::PixelLayout pixel_layout{ entry.pixel_format, xl7::graphics::ChannelOrder::RGBA };
        size_t stride = static_cast<size_t>( pixel_layout.stride );

        xl7::graphics::images::Image source_image{ { entry.pixel_format, xl7::graphics::ChannelOrder::RGBA, entry.source_width, entry.source_height, entry.source_depth }, entry.source_data, true };

        TESTLABS_CHECK_EQ( source_image.get_data().size(), source_image.get_desc().calculate_data_size() );

        TESTLABS_ASSERT_EQ( source_image.get_data().size(), entry.source_data.size() );

        xl7::graphics::images::Image target_image = xl7::graphics::images::ImageResizer::resize_image( source_image, entry.target_width, entry.target_height, entry.target_depth, entry.resampling_method );

        TESTLABS_CHECK( target_image.get_pixel_format() == entry.pixel_format );
        TESTLABS_CHECK( target_image.get_channel_order() == xl7::graphics::ChannelOrder::RGBA );
        TESTLABS_CHECK_EQ( target_image.get_width(), entry.target_width );
        TESTLABS_CHECK_EQ( target_image.get_height(), entry.target_height );
        TESTLABS_CHECK_EQ( target_image.get_depth(), entry.target_depth );

        TESTLABS_CHECK_EQ( target_image.get_data().size(), target_image.get_desc().calculate_data_size() );

        TESTLABS_ASSERT_EQ( target_image.get_data().size(), entry.target_data.size() );

        if ( pixel_layout.data_type == xl7::graphics::PixelLayout::DataType::FLOAT )
        {
            for ( size_t i = 0; i < target_image.get_data().size(); i += stride )
            {
                assert( i % sizeof(float) == 0 );
                size_t j = i / sizeof(float);
                TESTLABS_CHECK_EQ( ml7::round( ((const float*)target_image.get_data().data())[ j ], 4 ), ml7::round( ((const float*)entry.target_data.data())[ j ], 4 ) );
            }
        }
        else
        {
            cl7::byte_vector target_image_data{ target_image.get_data().data(), target_image.get_data().data() + target_image.get_data().size() };

            TESTLABS_CHECK_EQ( target_image_data, entry.target_data );
        }
    }
}
