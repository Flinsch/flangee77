
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/HasSelection.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/render/SelectionHelper.h>

#include "../../shared.h"
#include "./shared.h"



using f77_tests::gui::render::RecordingRenderer;



namespace {

    /** A plain Container, additionally opting into HasSelection for this file's tests. */
    class SelectableFace
        : public fl7::gui::Container
        , public fl7::gui::HasSelection
    {
    public:
        bool selection = false;
        size_t begin = 0;
        size_t end = 0;

        bool has_selection() const override { return selection; }
        size_t get_selection_begin_codepoint_index() const override { return begin; }
        size_t get_selection_end_codepoint_index() const override { return end; }

    }; // class SelectableFace

} // namespace



TESTLABS_CASE( u8"FaceLabs:  gui:  render:  SelectionHelper:  no-op while has_selection() is false, or the selection is empty (begin == end), or the face has no effective font" )
{
    RecordingRenderer renderer;
    fl7::gui::render::SelectionHelper helper;

    fl7::gui::Style style;
    style.font = &f77_tests::fonts::get_test_font();

    SelectableFace face;
    face.set_style_override(style);
    face.begin = 0;
    face.end = 3;
    // Selection left false: no selection to draw yet.

    helper.draw(&renderer, U"Hello", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});
    TESTLABS_CHECK( !renderer.last_rect.has_value() );

    face.selection = true;
    face.end = face.begin; // empty range.
    helper.draw(&renderer, U"Hello", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});
    TESTLABS_CHECK( !renderer.last_rect.has_value() );

    face.end = 3; // Non-empty now, but no font set on this one.
    SelectableFace fontless_face;
    fontless_face.selection = true;
    fontless_face.begin = 0;
    fontless_face.end = 3;
    helper.draw(&renderer, U"Hello", fontless_face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});
    TESTLABS_CHECK( !renderer.last_rect.has_value() );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  render:  SelectionHelper:  a non-empty selection on single-line text draws exactly one rect, styled per style.selection_color, wider for a larger selected range" )
{
    RecordingRenderer renderer;
    fl7::gui::render::SelectionHelper helper;

    fl7::gui::Style style;
    style.font = &f77_tests::fonts::get_test_font();
    style.selection_color = {0.2f, 0.4f, 0.6f, 0.8f};

    SelectableFace face;
    face.set_style_override(style);
    face.selection = true;
    face.begin = 0;
    face.end = 2;

    helper.draw(&renderer, U"Hello World", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});

    TESTLABS_ASSERT( renderer.last_rect.has_value() );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.r, 0.2f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_rect->color.g, 0.4f );
    TESTLABS_CHECK( renderer.last_rect->max.x > renderer.last_rect->min.x );
    TESTLABS_CHECK( renderer.last_rect->max.y > renderer.last_rect->min.y );

    const float narrow_width = renderer.last_rect->max.x - renderer.last_rect->min.x;

    face.end = 5; // Select more of the same text: should widen the highlight.
    helper.draw(&renderer, U"Hello World", face, {0.0f, 0.0f}, {}, {200.0f, 50.0f});

    TESTLABS_ASSERT( renderer.last_rect.has_value() );
    const float wider_width = renderer.last_rect->max.x - renderer.last_rect->min.x;
    TESTLABS_CHECK( wider_width > narrow_width );
}
