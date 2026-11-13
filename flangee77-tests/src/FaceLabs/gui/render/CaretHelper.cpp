
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/HasCaret.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/render/CaretHelper.h>

#include "../../shared.h"
#include "./shared.h"



using f77_tests::gui::render::RecordingRenderer;



namespace {

    /** A plain Container, additionally opting into HasCaret for this file's tests. */
    class CaretFace
        : public fl7::gui::Container
        , public fl7::gui::HasCaret
    {
    public:
        bool visible = false;
        size_t index = 0;

        bool is_caret_visible() const override { return visible; }
        size_t get_caret_codepoint_index() const override { return index; }

    }; // class CaretFace

} // namespace



TESTLABS_CASE( u8"FaceLabs:  gui:  render:  CaretHelper:  no-op while is_caret_visible() is false, or the face has no effective font" )
{
    RecordingRenderer renderer;
    fl7::gui::render::CaretHelper helper;

    fl7::gui::Style style;
    style.font = &f77_tests::fonts::get_test_font();

    CaretFace face;
    face.set_style_override(style);
    // visible left false.

    helper.draw(&renderer, U"Hello", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});
    TESTLABS_CHECK( !renderer.last_rect.has_value() );

    CaretFace fontless_face;
    fontless_face.visible = true;

    helper.draw(&renderer, U"Hello", fontless_face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});
    TESTLABS_CHECK( !renderer.last_rect.has_value() );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  CaretHelper:  while visible, draws exactly one thin rect, styled per style.text_style.text_color, further right for a later code point index" )
{
    RecordingRenderer renderer;
    fl7::gui::render::CaretHelper helper;

    fl7::gui::Style style;
    style.font = &f77_tests::fonts::get_test_font();
    style.text_style.text_color = {0.1f, 0.2f, 0.3f, 1.0f};

    CaretFace face;
    face.set_style_override(style);
    face.visible = true;
    face.index = 0;

    helper.draw(&renderer, U"Hello World", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});

    TESTLABS_ASSERT( renderer.last_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.r, 0.1f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.g, 0.2f );
    TESTLABS_CHECK( renderer.last_rect->max.x > renderer.last_rect->min.x ); // the fixed caret_width.
    TESTLABS_CHECK( renderer.last_rect->max.y > renderer.last_rect->min.y );

    const float x_at_start = renderer.last_rect->min.x;

    face.index = 5; // later in the same text: the caret should sit further right.
    helper.draw(&renderer, U"Hello World", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});

    TESTLABS_ASSERT( renderer.last_rect.has_value() );
    TESTLABS_CHECK( renderer.last_rect->min.x > x_at_start );
}
