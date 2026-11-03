
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/RadioGroup.h>
#include <FaceLabs/gui/faces/RadioButton.h>



TESTLABS_CASE( u8"FaceLabs:  gui:  RadioGroup:  ties together radio buttons regardless of their position in the Face tree: members under entirely unrelated parents are still mutually exclusive" )
{
    fl7::gui::RadioGroup group;

    fl7::gui::Container parent_a;
    fl7::gui::Container parent_b; // unrelated to parent_a: no common ancestor at all

    auto& button_a = parent_a.add_child<fl7::gui::faces::RadioButton>(&group, true);
    auto& button_b = parent_b.add_child<fl7::gui::faces::RadioButton>(&group);

    TESTLABS_CHECK( button_a.is_checked() );
    TESTLABS_CHECK( !button_b.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button_a );

    button_b.set_checked(true);

    TESTLABS_CHECK( !button_a.is_checked() );
    TESTLABS_CHECK( button_b.is_checked() );
    TESTLABS_CHECK( group.get_checked_button() == &button_b );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  RadioGroup:  two independent groups don't interfere with each other" )
{
    fl7::gui::RadioGroup group_a;
    fl7::gui::RadioGroup group_b;

    fl7::gui::faces::RadioButton button_a1(&group_a, true);
    fl7::gui::faces::RadioButton button_a2(&group_a);
    fl7::gui::faces::RadioButton button_b1(&group_b, true);

    button_a2.set_checked(true); // only affects group_a

    TESTLABS_CHECK( group_a.get_checked_button() == &button_a2 );
    TESTLABS_CHECK( group_b.get_checked_button() == &button_b1 );
    TESTLABS_CHECK( button_b1.is_checked() ); // untouched
}

TESTLABS_CASE( u8"FaceLabs:  gui:  RadioGroup:  destroying a member that isn't the currently checked one leaves the group's checked member untouched" )
{
    fl7::gui::RadioGroup group;
    fl7::gui::faces::RadioButton button_a(&group, true);

    {
        fl7::gui::faces::RadioButton button_b(&group); // never checked
        TESTLABS_CHECK( group.get_checked_button() == &button_a );
    } // button_b destroyed here, unchecked: must not clear the group's checked member

    TESTLABS_CHECK( group.get_checked_button() == &button_a );
    TESTLABS_CHECK( button_a.is_checked() );
}
