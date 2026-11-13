
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/render/ImageHelper.h>

#include "./shared.h"



using f77_tests::gui::render::RecordingRenderer;



// #############################################################################
// draw(): takes an explicit box (position/size), not a face's own bounds
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  ImageHelper:  Stretch fills the given box exactly, wherever that box happens to be (e.g., inset by padding, not necessarily at a face's own position/size)" )
{
    RecordingRenderer renderer;
    fl7::gui::render::ImageHelper helper;

    fl7::gui::Image image;
    image.native_size = {64.0f, 32.0f}; // irrelevant to Stretch.

    helper.draw(&renderer, image, {10.0f, 20.0f}, {100.0f, 50.0f}, fl7::gui::ImageScaleMode::Stretch);

    TESTLABS_ASSERT( renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.y, 70.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  ImageHelper:  Zoom scales the image uniformly to fit within the box, centered, against the box's own position/size" )
{
    RecordingRenderer renderer;
    fl7::gui::render::ImageHelper helper;

    fl7::gui::Image image;
    image.native_size = {64.0f, 32.0f}; // 2:1 aspect ratio.

    // A 100x100 box: height is the limiting axis (scale = 100/32), giving a
    // 200x100 image clamped by width instead (scale = 100/64 = 1.5625), i.e.,
    // the smaller of the two: 100x50, centered (25px letterboxed top/bottom).
    helper.draw(&renderer, image, {0.0f, 0.0f}, {100.0f, 100.0f}, fl7::gui::ImageScaleMode::Zoom);

    TESTLABS_ASSERT( renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.y, 25.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.y, 75.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  ImageHelper:  Center draws the image at its own native size, centered within the box" )
{
    RecordingRenderer renderer;
    fl7::gui::render::ImageHelper helper;

    fl7::gui::Image image;
    image.native_size = {20.0f, 10.0f};

    helper.draw(&renderer, image, {50.0f, 50.0f}, {100.0f, 100.0f}, fl7::gui::ImageScaleMode::Center);

    TESTLABS_ASSERT( renderer.last_textured_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.x, 90.0f ); // 50 + (100 - 20) / 2.
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->min.y, 95.0f ); // 50 + (100 - 10) / 2.
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_textured_rect->max.y, 105.0f );
}
