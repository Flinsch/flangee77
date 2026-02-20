
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/ImageConverter.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  ImageConverter" )
{
    // This is just so that we notice when a value is inserted or removed
    // and then we should also adjust the tests accordingly.
    TESTLABS_ASSERT_EQ( static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ), 55 );


    struct Entry
    {
        xl7::graphics::PixelFormat pixel_format;
        xl7::graphics::ChannelOrder channel_order;
    } entry1, entry2;

    std::vector<Entry> container;
    for ( unsigned pixel_format = 1; pixel_format <= static_cast<unsigned>( xl7::graphics::PixelFormat::A8_UNORM ); ++pixel_format )
    {
        if ( xl7::graphics::PixelFormat( pixel_format ) == xl7::graphics::PixelFormat::R11G11B10_FLOAT )
            continue;
        for ( unsigned channel_order = 0; channel_order < 4; ++channel_order )
            container.push_back( { xl7::graphics::PixelFormat( pixel_format ), xl7::graphics::ChannelOrder( channel_order ) } );
    }

    const xl7::graphics::Color image_colors[4] = {
        { 0.9f, 0.3f, 0.1f, 0.5f },
        { 0.1f, 0.9f, 0.3f, 0.5f },
        { 0.3f, 0.1f, 0.9f, 0.5f },
        { 0.75f, 0.75f, 0.25f, 0.5f },
    };


    constexpr cl7::u8char_t rgbakeys[4] = { u8'R', u8'G', u8'B', u8'A' };

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


    TESTLABS_SUBCASE_BATCH( u8"convert_image", container, entry1 )
    {
        xl7::graphics::PixelLayout pixel_layout1{ entry1.pixel_format, entry1.channel_order };
        size_t stride1 = static_cast<size_t>( pixel_layout1.stride );

        cl7::byte_vector source_data{ stride1 * 4 };
        xl7::graphics::images::ImageConverter::pack_color( image_colors[0], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 0, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[1], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 1, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[2], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 2, stride1 } );
        xl7::graphics::images::ImageConverter::pack_color( image_colors[3], entry1.pixel_format, entry1.channel_order, { source_data.data() + stride1 * 3, stride1 } );

        xl7::graphics::images::Image source_image{ { entry1.pixel_format, entry1.channel_order, 2, 2, 1 }, source_data, true };

        TESTLABS_ASSERT_EQ( source_image.get_data().size(), 4 * stride1 );

        TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"convert_image", container, entry2, cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) )
        {
            xl7::graphics::PixelLayout pixel_layout2{ entry2.pixel_format, entry2.channel_order };
            size_t stride2 = static_cast<size_t>( pixel_layout2.stride );

            xl7::graphics::images::Image target_image = xl7::graphics::images::ImageConverter::convert_image( source_image, entry2.pixel_format, entry2.channel_order );

            TESTLABS_CHECK( target_image.get_pixel_format() == entry2.pixel_format );
            TESTLABS_CHECK( target_image.get_channel_order() == entry2.channel_order );
            TESTLABS_CHECK_EQ( target_image.get_width(), 2 );
            TESTLABS_CHECK_EQ( target_image.get_height(), 2 );
            TESTLABS_CHECK_EQ( target_image.get_depth(), 1 );

            TESTLABS_CHECK_EQ( target_image.get_data().size(), target_image.get_desc().calculate_data_size() );

            TESTLABS_ASSERT_EQ( target_image.get_data().size(), 4 * stride2 );

            const auto& target_data = target_image.get_data();
            xl7::graphics::Color colors[4] = {
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 0, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 1, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 2, stride2 }, entry2.pixel_format, entry2.channel_order ),
                xl7::graphics::images::ImageConverter::unpack_color( { target_data.data() + stride2 * 3, stride2 }, entry2.pixel_format, entry2.channel_order ),
            };

            for ( unsigned rgba = 0; rgba < 4; ++rgba )
            {
                TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) + u8"  (" + rgbakeys[rgba] + u8")" );

                constexpr signed rgba_map[5][5][4] = {
                    {
                        { -1, -1, -1,  3 }, // A    -> A
                        {  3,  3,  3, -1 }, // A    -> R
                        {  3,  3, -1, -1 }, // A    -> RG
                        {  3,  3,  3, -1 }, // A    -> RGB
                        { -1, -1, -1,  3 }, // A    -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // R    -> A
                        {  0,  0,  0, -1 }, // R    -> R
                        {  0,  0, -1, -1 }, // R    -> RG
                        {  0,  0,  0, -1 }, // R    -> RGB
                        {  0,  0,  0, -1 }, // R    -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // RG   -> A
                        {  0,  0,  0, -1 }, // RG   -> R
                        {  0,  1, -1, -1 }, // RG   -> RG
                        {  0,  1, -1, -1 }, // RG   -> RGB
                        {  0,  1, -1, -1 }, // RG   -> RGBA
                    },
                    {
                        { -1, -1, -1,  0 }, // RGB  -> A
                        {  0,  0,  0, -1 }, // RGB  -> R
                        {  0,  1, -1, -1 }, // RGB  -> RG
                        {  0,  1,  2, -1 }, // RGB  -> RGB
                        {  0,  1,  2, -1 }, // RGB  -> RGBA
                    },
                    {
                        { -1, -1, -1,  3 }, // RGBA -> A
                        {  0,  0,  0, -1 }, // RGBA -> R
                        {  0,  1, -1, -1 }, // RGBA -> RG
                        {  0,  1,  2, -1 }, // RGBA -> RGB
                        {  0,  1,  2,  3 }, // RGBA -> RGBA
                    },
                };

                auto _from_to = [](const xl7::graphics::PixelLayout& pixel_layout) -> unsigned {
                    if ( pixel_layout.channel_count == 1 && pixel_layout.a.depth )
                        return 0;
                    return pixel_layout.channel_count;
                };

                signed mapped_rgba = rgba_map[ _from_to(pixel_layout1) ][ _from_to(pixel_layout2) ][ rgba ];
                if ( mapped_rgba < 0 )
                {
                    TESTLABS_CHECK_EQ( colors[0][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[1][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[2][rgba], xl7::graphics::Color::BLACK[rgba] );
                    TESTLABS_CHECK_EQ( colors[3][rgba], xl7::graphics::Color::BLACK[rgba] );
                }
                else
                {
                    unsigned rgba_ = static_cast<unsigned>( mapped_rgba );
                    assert( rgba_ < 4 );

                    if ( rgba_ != rgba )
                        TESTLABS_SUBCASE_DATA_STRING( cl7::to_string( entry1.pixel_format ) + u8" " + cl7::to_string( entry1.channel_order ) + u8" -> " + cl7::to_string( entry2.pixel_format ) + u8" " + cl7::to_string( entry2.channel_order ) + u8"  (" + rgbakeys[rgba_] + u8" -> " + rgbakeys[rgba] + u8")" );

                    unsigned depth1 = pixel_layout1.channels[ rgba_ ].depth;
                    assert( depth1 );
                    unsigned depth2 = pixel_layout2.channels[ rgba ].depth;

                    unsigned prec = ml7::min2( _precision( depth1 ), _precision( depth2 ) );

                    TESTLABS_CHECK_EQ( ml7::round( colors[0][rgba], prec ), ml7::round( image_colors[0][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[1][rgba], prec ), ml7::round( image_colors[1][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[2][rgba], prec ), ml7::round( image_colors[2][rgba_], prec ) );
                    TESTLABS_CHECK_EQ( ml7::round( colors[3][rgba], prec ), ml7::round( image_colors[3][rgba_], prec ) );
                }
            }
        }
    }
}
