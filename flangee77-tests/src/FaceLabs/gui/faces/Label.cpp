
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/faces/Label.h>



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
