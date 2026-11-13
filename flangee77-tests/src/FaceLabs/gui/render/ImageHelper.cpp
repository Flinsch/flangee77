
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/render/ImageHelper.h>
#include <FaceLabs/gui/render/AbstractRenderer.h>

#include <optional>



namespace {

    /** Records the last draw_textured_rect() call's destination rect; everything else is a no-op. */
    class RecordingRenderer
        : public fl7::gui::render::AbstractRenderer
    {
    public:
        std::optional<ml7::Vector2f> last_min;
        std::optional<ml7::Vector2f> last_max;

    private:
        void _begin_frame_impl() override {}
        void _end_frame_impl() override {}

        void _render_faces_impl(const std::vector<std::unique_ptr<fl7::gui::Face>>& top_level_faces) override {}

        void _draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color) override {}
        void _draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint) override
        {
            last_min = position_min;
            last_max = position_max;
        }
        void _draw_text_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f position) override {}
        void _draw_text_in_box_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size) override {}

        void _push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max) override {}
        void _pop_clip_rect_impl() override {}

    }; // class RecordingRenderer

} // namespace



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

    TESTLABS_ASSERT( renderer.last_min.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->y, 70.0f );
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

    TESTLABS_ASSERT( renderer.last_min.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->y, 25.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->y, 75.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  ImageHelper:  Center draws the image at its own native size, centered within the box" )
{
    RecordingRenderer renderer;
    fl7::gui::render::ImageHelper helper;

    fl7::gui::Image image;
    image.native_size = {20.0f, 10.0f};

    helper.draw(&renderer, image, {50.0f, 50.0f}, {100.0f, 100.0f}, fl7::gui::ImageScaleMode::Center);

    TESTLABS_ASSERT( renderer.last_min.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->x, 90.0f ); // 50 + (100 - 20) / 2.
    TESTLABS_CHECK_EQ_FLT( renderer.last_min->y, 95.0f ); // 50 + (100 - 10) / 2.
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->x, 110.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_max->y, 105.0f );
}
