
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/faces/PictureBox.h>



// #############################################################################
// Plain API (no Shell/input involved; PictureBox has no mouse/key behavior of its own)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  PictureBox:  default-constructed has no image; the image constructor argument sets the initial one" )
{
    fl7::gui::faces::PictureBox empty_box;
    TESTLABS_CHECK( empty_box.get_image() == nullptr );

    fl7::gui::Image image;
    image.native_size = {64.0f, 32.0f};

    fl7::gui::faces::PictureBox box(image);
    TESTLABS_ASSERT( box.get_image() != nullptr );
    TESTLABS_CHECK_EQ_FLT( box.get_image()->native_size.x, 64.0f );
    TESTLABS_CHECK_EQ_FLT( box.get_image()->native_size.y, 32.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  PictureBox:  set_image()/get_image(), including clearing it back to nullptr via nullopt" )
{
    fl7::gui::faces::PictureBox box;
    TESTLABS_CHECK( box.get_image() == nullptr );

    fl7::gui::Image image;
    image.native_size = {16.0f, 16.0f};
    box.set_image(image);
    TESTLABS_ASSERT( box.get_image() != nullptr );
    TESTLABS_CHECK_EQ_FLT( box.get_image()->native_size.x, 16.0f );

    box.set_image(std::nullopt);
    TESTLABS_CHECK( box.get_image() == nullptr );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  PictureBox:  get_image_scale_mode() defaults to Stretch; set_image_scale_mode()/get_image_scale_mode()" )
{
    fl7::gui::faces::PictureBox box;
    TESTLABS_CHECK( box.get_image_scale_mode() == fl7::gui::ImageScaleMode::Stretch );

    box.set_image_scale_mode(fl7::gui::ImageScaleMode::Zoom);
    TESTLABS_CHECK( box.get_image_scale_mode() == fl7::gui::ImageScaleMode::Zoom );

    box.set_image_scale_mode(fl7::gui::ImageScaleMode::Center);
    TESTLABS_CHECK( box.get_image_scale_mode() == fl7::gui::ImageScaleMode::Center );
}
