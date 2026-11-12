
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/faces/Label.h>

#include "../../shared.h"



namespace {

    using f77_tests::fonts::get_test_font;

} // namespace



// #############################################################################
// Plain API (no Shell/input involved; Label has no mouse/key behavior of its own)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  default-constructed is empty; the text constructor argument sets the initial text; get_display_text() always mirrors get_text()" )
{
    fl7::gui::faces::Label empty_label;
    TESTLABS_CHECK( empty_label.get_text().empty() );
    TESTLABS_CHECK( empty_label.get_display_text().empty() );

    fl7::gui::faces::Label label(U"Hello");
    TESTLABS_CHECK_EQ( label.get_text(), cl7::u32string(U"Hello") );
    TESTLABS_CHECK_EQ( label.get_display_text(), cl7::u32string(U"Hello") );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  set_text()/get_text()" )
{
    fl7::gui::faces::Label label;

    label.set_text(U"Caption");
    TESTLABS_CHECK_EQ( label.get_text(), cl7::u32string(U"Caption") );
    TESTLABS_CHECK_EQ( label.get_display_text(), cl7::u32string(U"Caption") );

    label.set_text(U"Renamed");
    TESTLABS_CHECK_EQ( label.get_text(), cl7::u32string(U"Renamed") );
    TESTLABS_CHECK_EQ( label.get_display_text(), cl7::u32string(U"Renamed") );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  without an explicit override, get_preferred_size()/get_minimum_size() stay {0, 0} regardless of font; get_effective_preferred_size()/get_effective_minimum_size() need an effective font to calculate anything, else also {0, 0}" )
{
    fl7::gui::faces::Label label(U"Hello");
    TESTLABS_CHECK_EQ_FLT( label.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_minimum_size().y, 0.0f );

    TESTLABS_CHECK_EQ_FLT( label.get_effective_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_preferred_size().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().y, 0.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  set_style_override() with a real font: get_effective_preferred_size()/get_effective_minimum_size() both reflect the text's own natural (unwrapped) size, growing with more text/lines; get_preferred_size()/get_minimum_size() stay {0, 0} regardless (no explicit override set)" )
{
    fl7::gui::Style style;
    style.font = &get_test_font();

    fl7::gui::faces::Label label(U"Hi");
    label.set_style_override(style);

    const ml7::Vector2f short_size = label.get_effective_preferred_size();
    TESTLABS_CHECK( short_size.x > 0.0f );
    TESTLABS_CHECK( short_size.y > 0.0f );
    // Label's text can't usefully be laid out any smaller than its own natural size.
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().x, short_size.x );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().y, short_size.y );
    // No explicit override anywhere above: the plain getters stay untouched.
    TESTLABS_CHECK_EQ_FLT( label.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_minimum_size().x, 0.0f );

    label.set_text(U"Hello, much longer text than before");
    TESTLABS_CHECK( label.get_effective_preferred_size().x > short_size.x );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_preferred_size().y, short_size.y ); // Still a single line.

    label.set_text(U"Line one\nLine two");
    TESTLABS_CHECK( label.get_effective_preferred_size().y > short_size.y ); // Two lines now.
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  set_preferred_size()/set_minimum_size() set exactly what get_preferred_size()/get_minimum_size() then return (no merging); get_effective_preferred_size()/get_effective_minimum_size() merge them with the calculated size instead, independently per component (<= 0 defers to the calculated size)" )
{
    fl7::gui::Style style;
    style.font = &get_test_font();

    fl7::gui::faces::Label label(U"Hi");
    label.set_style_override(style);

    const ml7::Vector2f calculated = label.get_effective_preferred_size();

    label.set_preferred_size({100.0f, 0.0f}); // x overridden, y deferred (<= 0).
    TESTLABS_CHECK_EQ_FLT( label.get_preferred_size().x, 100.0f ); // Exactly what was set...
    TESTLABS_CHECK_EQ_FLT( label.get_preferred_size().y, 0.0f ); // ...not merged with anything.
    TESTLABS_CHECK_EQ_FLT( label.get_effective_preferred_size().x, 100.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_preferred_size().y, calculated.y ); // Deferred component: falls back here.

    label.set_minimum_size({0.0f, 50.0f}); // x deferred (<= 0), y overridden.
    TESTLABS_CHECK_EQ_FLT( label.get_minimum_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_minimum_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().x, calculated.x );
    TESTLABS_CHECK_EQ_FLT( label.get_effective_minimum_size().y, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  faces:  Label:  get_effective_preferred_size()/get_effective_minimum_size() add the effective style's own padding on top of the text's own natural size (minimum == preferred for Label either way)" )
{
    fl7::gui::Style unpadded_style;
    unpadded_style.font = &get_test_font();

    fl7::gui::Style padded_style = unpadded_style;
    padded_style.padding = fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f); // left/right 4, top/bottom 2.

    fl7::gui::faces::Label unpadded_label(U"Hi");
    unpadded_label.set_style_override(unpadded_style);
    fl7::gui::faces::Label padded_label(U"Hi");
    padded_label.set_style_override(padded_style);

    const ml7::Vector2f unpadded = unpadded_label.get_effective_preferred_size();
    const ml7::Vector2f padded = padded_label.get_effective_preferred_size();

    TESTLABS_CHECK_EQ_FLT( padded.x, unpadded.x + 8.0f ); // + left(4) + right(4)
    TESTLABS_CHECK_EQ_FLT( padded.y, unpadded.y + 4.0f ); // + top(2) + bottom(2)
    TESTLABS_CHECK_EQ_FLT( padded_label.get_effective_minimum_size().x, padded.x );
    TESTLABS_CHECK_EQ_FLT( padded_label.get_effective_minimum_size().y, padded.y );

    // The plain getters, as ever, stay untouched regardless: no override set on either.
    TESTLABS_CHECK_EQ_FLT( padded_label.get_preferred_size().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( padded_label.get_minimum_size().x, 0.0f );
}
