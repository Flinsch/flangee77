
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/layout/DefaultLayout.h>



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  a child with the default anchor (Left|Top) is left exactly where it is as the container is resized, same as having no layout at all" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 200.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({10.0f, 10.0f});
    child.set_size({50.0f, 50.0f});

    container.set_layout<fl7::gui::layout::DefaultLayout>();
    container.set_size({300.0f, 300.0f});

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_position().y, 10.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 50.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().y, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  Right-only anchor keeps the child's distance from the right edge as the container grows" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({150.0f, 10.0f});
    child.set_size({40.0f, 20.0f}); // 10px from the right edge (200 - (150+40)).
    child.set_anchor(fl7::gui::Anchor::Right);

    container.set_layout<fl7::gui::layout::DefaultLayout>();
    container.set_size({300.0f, 100.0f});

    // Still 10px from the (now further right) right edge; size untouched.
    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 250.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 40.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  Left+Right anchor stretches the child to keep its distance from both edges as the container grows" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({10.0f, 10.0f});
    child.set_size({150.0f, 20.0f}); // 10px left, 40px right (200 - (10+150)).
    child.set_anchor(fl7::gui::Anchor::Left | fl7::gui::Anchor::Right);

    container.set_layout<fl7::gui::layout::DefaultLayout>();
    container.set_size({300.0f, 100.0f});

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 250.0f ); // 300 - 10 - 40.
}
