
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/render/TextHelper.h>

#include "../../shared.h"
#include "./shared.h"



using f77_tests::gui::render::RecordingRenderer;



TESTLABS_CASE( u8"FaceLabs:  gui:  render:  TextHelper:  draws text at absolute_position minus text_scroll_offset, laid out against text_box_size, using the face's own effective font/text style" )
{
    RecordingRenderer renderer;
    fl7::gui::render::TextHelper helper;

    fl7::gui::Container face;
    fl7::gui::Style style;
    style.font = &f77_tests::fonts::get_test_font();
    style.text_style.text_color = {1.0f, 0.0f, 0.0f, 1.0f};
    face.set_style_override(style);

    helper.draw(&renderer, U"Hello", face, {10.0f, 20.0f}, {2.0f, 3.0f}, {100.0f, 50.0f});

    TESTLABS_ASSERT( renderer.last_text_in_box.has_value() );
    TESTLABS_CHECK_EQ( renderer.last_text_in_box->text, cl7::u32string(U"Hello") );
    TESTLABS_CHECK( renderer.last_text_in_box->font == style.font );
    TESTLABS_CHECK_EQ_FLT( renderer.last_text_in_box->text_style.text_color.r, 1.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_text_in_box->box_position.x, 8.0f ); // 10 - 2.
    TESTLABS_CHECK_EQ_FLT( renderer.last_text_in_box->box_position.y, 17.0f ); // 20 - 3.
    TESTLABS_CHECK_EQ_FLT( renderer.last_text_in_box->box_size.x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( renderer.last_text_in_box->box_size.y, 50.0f );
}
