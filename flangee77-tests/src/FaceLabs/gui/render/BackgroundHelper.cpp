
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/NineSliceChrome.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/render/BackgroundHelper.h>

#include "./shared.h"



using f77_tests::gui::render::RecordingRenderer;



TESTLABS_CASE( u8"FaceLabs:  gui:  render:  BackgroundHelper:  without a chrome, draws exactly one flat rect over the face's full bounds, styled per style.background_color" )
{
    RecordingRenderer renderer;
    fl7::gui::render::BackgroundHelper helper;

    fl7::gui::Container face;
    face.set_size({100.0f, 50.0f});

    fl7::gui::Style style;
    style.background_color = {0.1f, 0.2f, 0.3f, 0.4f};
    face.set_style_override(style);

    helper.draw(&renderer, face, {10.0f, 20.0f});

    TESTLABS_ASSERT( renderer.last_rect.has_value() );
    TESTLABS_CHECK( !renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->min.x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->min.y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->max.x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->max.y, 70.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.r, 0.1f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.g, 0.2f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  BackgroundHelper:  a chrome with zero insets collapses to a single textured rect over the face's full bounds, at the sprite's native UVs" )
{
    RecordingRenderer renderer;
    fl7::gui::render::BackgroundHelper helper;

    fl7::gui::Container face;
    face.set_size({100.0f, 50.0f});

    fl7::gui::NineSliceChrome chrome;
    chrome.texture_id = xl7::graphics::textures::Texture2D::Id(7, 1);
    chrome.uv_min = {0.25f, 0.25f};
    chrome.uv_max = {0.75f, 0.75f};
    chrome.sprite_size = {40.0f, 40.0f};
    // Insets left at 0: no border, so the 3x3 grid collapses to just the center cell.

    fl7::gui::Style style;
    style.chrome = &chrome;
    face.set_style_override(style);

    helper.draw(&renderer, face, {10.0f, 20.0f});

    TESTLABS_ASSERT( renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ( renderer.last_textured_rect->texture_id, chrome.texture_id );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.y, 70.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_min.x, chrome.uv_min.x );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_min.y, chrome.uv_min.y );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_max.x, chrome.uv_max.x );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_max.y, chrome.uv_max.y );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  BackgroundHelper:  a chrome with insets places its bottom-right corner cell at native size in the destination's own corner, sampling the sprite's bottom-right inset region" )
{
    RecordingRenderer renderer;
    fl7::gui::render::BackgroundHelper helper;

    fl7::gui::Container face;
    face.set_size({100.0f, 50.0f});

    fl7::gui::NineSliceChrome chrome;
    chrome.texture_id = xl7::graphics::textures::Texture2D::Id(3, 1);
    chrome.uv_min = {0.0f, 0.0f};
    chrome.uv_max = {1.0f, 1.0f};
    chrome.sprite_size = {20.0f, 20.0f};
    chrome.inset_left = 4.0f;
    chrome.inset_top = 4.0f;
    chrome.inset_right = 6.0f;
    chrome.inset_bottom = 8.0f;

    fl7::gui::Style style;
    style.chrome = &chrome;
    face.set_style_override(style);

    helper.draw(&renderer, face, {10.0f, 20.0f});

    // The last 9-sliced quad drawn is the bottom-right corner: native inset_right x inset_bottom
    // size, flush with the destination's own bottom-right corner.
    TESTLABS_ASSERT( renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ( renderer.last_textured_rect->texture_id, chrome.texture_id );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.x, 10.0f + 100.0f - 6.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.y, 20.0f + 50.0f - 8.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.x, 10.0f + 100.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.y, 20.0f + 50.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_min.x, 1.0f - 6.0f / 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_min.y, 1.0f - 8.0f / 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_max.x, 1.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->uv_max.y, 1.0f );
}
