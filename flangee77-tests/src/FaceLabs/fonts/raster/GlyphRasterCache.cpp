
#include <TestLabs/TestSuite.h>

#include <FaceLabs/fonts/raster/GlyphRasterCache.h>
#include <FaceLabs/fonts/raster/SdfRasterizer.h>
#include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>
#include <FaceLabs/fonts/Font.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/text/format.h>
#include <CoreLabs/io/File.h>

#include "../../../shared.h"



namespace {

    fl7::fonts::Font& get_test_font()
    {
        static fl7::fonts::Font font(std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(
            cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf"));
        return font;
    }

    cl7::u8string make_scratch_cache_directory(cl7::u8string_view name)
    {
        return cl7::platform::filesystem::get_temp_directory() + u8"flangee77_glyph_raster_cache_test/" + cl7::u8string(name) + u8"/";
    }

} // namespace



TESTLABS_CASE( u8"FaceLabs:  fonts:  raster:  GlyphRasterCache:  store/try_load - round-trip" )
{
    fl7::fonts::raster::GlyphRasterCache cache{make_scratch_cache_directory(u8"round_trip")};

    const cl7::byte_vector data = cl7::make_bytes(0x00, 0x40, 0x80, 0xc0, 0xff, 0x10, 0x20, 0x30);
    xl7::graphics::images::ImageDesc desc{
        .pixel_format = xl7::graphics::PixelFormat::R8_UNORM,
        .channel_order = xl7::graphics::ChannelOrder::RGBA,
        .width = 4,
        .height = 2,
        .depth = 1,
    };
    fl7::fonts::raster::RasterResult original{
        .glyph_image = {desc, data},
        .pixel_offset = {.left = -3, .top = -11},
    };

    const cl7::text::codec::codepoint codepoint = {static_cast<unsigned>('A')};
    const size_t key = 0x0123456789abcdefull;
    cache.store(codepoint, key, original);

    std::optional<fl7::fonts::raster::RasterResult> loaded = cache.try_load(codepoint, key);
    TESTLABS_ASSERT( loaded.has_value() );

    TESTLABS_CHECK_EQ( loaded->glyph_image.get_width(), original.glyph_image.get_width() );
    TESTLABS_CHECK_EQ( loaded->glyph_image.get_height(), original.glyph_image.get_height() );
    TESTLABS_CHECK_EQ( loaded->glyph_image.get_depth(), original.glyph_image.get_depth() );
    TESTLABS_CHECK_EQ( loaded->glyph_image.get_pixel_format(), original.glyph_image.get_pixel_format() );
    TESTLABS_CHECK_EQ( loaded->glyph_image.get_channel_order(), original.glyph_image.get_channel_order() );
    TESTLABS_CHECK_EQ( cl7::to_bytes(loaded->glyph_image.get_data()), data );
    TESTLABS_CHECK_EQ( loaded->pixel_offset.left, original.pixel_offset.left );
    TESTLABS_CHECK_EQ( loaded->pixel_offset.top, original.pixel_offset.top );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  raster:  GlyphRasterCache:  try_load - miss for an unstored key" )
{
    fl7::fonts::raster::GlyphRasterCache cache{make_scratch_cache_directory(u8"miss")};

    std::optional<fl7::fonts::raster::RasterResult> loaded = cache.try_load({static_cast<unsigned>('A')}, 0xdeadbeefull);
    TESTLABS_CHECK( !loaded.has_value() );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  raster:  GlyphRasterCache:  try_load - corrupt entry is treated as a miss" )
{
    const cl7::u8string cache_directory = make_scratch_cache_directory(u8"corrupt");
    fl7::fonts::raster::GlyphRasterCache cache{cache_directory};

    const cl7::byte_vector data = cl7::make_bytes(0x11, 0x22, 0x33, 0x44);
    xl7::graphics::images::ImageDesc desc{
        .pixel_format = xl7::graphics::PixelFormat::R8_UNORM,
        .channel_order = xl7::graphics::ChannelOrder::RGBA,
        .width = 2,
        .height = 2,
        .depth = 1,
    };
    fl7::fonts::raster::RasterResult original{
        .glyph_image = {desc, data},
        .pixel_offset = {.left = 0, .top = 0},
    };

    const cl7::text::codec::codepoint codepoint = {static_cast<unsigned>('A')};
    const size_t key = 0x1111222233334444ull;
    cache.store(codepoint, key, original);

    // Overwrite the entry's file directly with garbage, at the exact path
    // `GlyphRasterCache` itself would compute for this code point and key.
    const cl7::u8string file_path = cache_directory + u8"0041-" + cl7::u8string(cl7::text::format::to_hex_lc(key, 16)) + u8".glyphcache";
    cl7::io::File file(file_path, cl7::io::OpenMode::Truncate);
    TESTLABS_ASSERT( file.is_writable() );
    const cl7::byte_vector garbage = cl7::make_bytes(0x01, 0x02, 0x03);
    file.write(garbage);
    file.close();

    std::optional<fl7::fonts::raster::RasterResult> loaded = cache.try_load(codepoint, key);
    TESTLABS_CHECK( !loaded.has_value() );
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  raster:  GlyphRasterCache:  filenames carry the codepoint and an optional prefix" )
{
    xl7::graphics::images::ImageDesc desc{
        .pixel_format = xl7::graphics::PixelFormat::R8_UNORM,
        .channel_order = xl7::graphics::ChannelOrder::RGBA,
        .width = 1,
        .height = 2,
        .depth = 1,
    };
    fl7::fonts::raster::RasterResult result{
        .glyph_image = {desc, cl7::make_bytes(0xaa, 0xbb)},
        .pixel_offset = {.left = 0, .top = 0},
    };

    const cl7::text::codec::codepoint codepoint = {static_cast<unsigned>('Q')};
    const size_t key = 0x2222333344445555ull;
    const cl7::u8string hash = cl7::u8string(cl7::text::format::to_hex_lc(key, 16));

    // No prefix: just "<codepoint>-<hash>.glyphcache".
    {
        const cl7::u8string cache_directory = make_scratch_cache_directory(u8"filename_no_prefix");
        fl7::fonts::raster::GlyphRasterCache cache{cache_directory};
        cache.store(codepoint, key, result);

        TESTLABS_CHECK( cl7::io::File(cache_directory + u8"0051-" + hash + u8".glyphcache", cl7::io::OpenMode::Read).is_readable() );
    }

    // With a prefix: used verbatim, right before the codepoint, no separator inserted.
    {
        const cl7::u8string cache_directory = make_scratch_cache_directory(u8"filename_with_prefix");
        fl7::fonts::raster::GlyphRasterCache cache{cache_directory, u8"MSDF-NotoSans-Regular-"};
        cache.store(codepoint, key, result);

        TESTLABS_CHECK( cl7::io::File(cache_directory + u8"MSDF-NotoSans-Regular-0051-" + hash + u8".glyphcache", cl7::io::OpenMode::Read).is_readable() );
    }
}

TESTLABS_CASE( u8"FaceLabs:  fonts:  raster:  GlyphRasterCache:  SdfRasterizer integration - cache hit across separate instances" )
{
    auto& font = get_test_font();
    const fl7::fonts::Glyph* glyph = font.access().find_glyph({static_cast<unsigned>('A')});
    TESTLABS_ASSERT( glyph != nullptr );

    const fl7::fonts::raster::RasterSizeConfig size_config{.font_size = 32.0f, .padding = 1};
    const cl7::u8string cache_directory = make_scratch_cache_directory(u8"sdf_integration");

    fl7::fonts::raster::RasterResult first_result;
    {
        fl7::fonts::raster::GlyphRasterCache cache{cache_directory};
        fl7::fonts::raster::SdfRasterizer rasterizer;
        rasterizer.set_cache(&cache);
        first_result = rasterizer.rasterize_glyph(*glyph, size_config);
    }

    // Simulate a new process: fresh rasterizer and cache instances, same directory.
    fl7::fonts::raster::RasterResult second_result;
    {
        fl7::fonts::raster::GlyphRasterCache cache{cache_directory};
        fl7::fonts::raster::SdfRasterizer rasterizer;
        rasterizer.set_cache(&cache);
        second_result = rasterizer.rasterize_glyph(*glyph, size_config);
    }

    TESTLABS_CHECK_EQ( second_result.glyph_image.get_width(), first_result.glyph_image.get_width() );
    TESTLABS_CHECK_EQ( second_result.glyph_image.get_height(), first_result.glyph_image.get_height() );
    TESTLABS_CHECK_EQ( second_result.glyph_image.get_pixel_format(), first_result.glyph_image.get_pixel_format() );
    TESTLABS_CHECK_EQ( second_result.glyph_image.get_channel_order(), first_result.glyph_image.get_channel_order() );
    TESTLABS_CHECK_EQ( cl7::to_bytes(second_result.glyph_image.get_data()), cl7::to_bytes(first_result.glyph_image.get_data()) );
    TESTLABS_CHECK_EQ( second_result.pixel_offset.left, first_result.pixel_offset.left );
    TESTLABS_CHECK_EQ( second_result.pixel_offset.top, first_result.pixel_offset.top );
}
