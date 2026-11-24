
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

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  Left+Right anchor never actually shrinks the child below its own effective minimum width, overflowing the container instead" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_position({10.0f, 10.0f});
    child.set_size({150.0f, 20.0f}); // 10px left, 40px right (200 - (10+150)).
    child.set_minimum_size({100.0f, 5.0f});
    child.set_anchor(fl7::gui::Anchor::Left | fl7::gui::Anchor::Right);

    container.set_layout<fl7::gui::layout::DefaultLayout>();

    // Shrinking to 120px would otherwise leave only 120 - 10 (left) - 40
    // (right) = 70px for the child, below its own 100px minimum.
    container.set_size({120.0f, 100.0f});

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 10.0f ); // Still flush left.
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 100.0f ); // Its own minimum, not 70; overflows the container's own right edge instead.
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  calculate_minimum_size() is, per axis, the largest of every Left+Right (or Top+Bottom) filled child's own required container extent" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& filled_child = container.add_child<fl7::gui::Container>();
    filled_child.set_position({10.0f, 10.0f});
    filled_child.set_size({150.0f, 20.0f}); // 10px left, 40px right.
    filled_child.set_minimum_size({100.0f, 5.0f});
    filled_child.set_anchor(fl7::gui::Anchor::All);

    // Anchored on only one side (Right alone): never actually resizes on
    // this axis at all (see this layout's own doc comment), so it doesn't
    // contribute to calculate_minimum_size() either, regardless of how
    // large its own minimum size is.
    auto& one_sided_child = container.add_child<fl7::gui::Container>();
    one_sided_child.set_position({180.0f, 10.0f});
    one_sided_child.set_size({10.0f, 10.0f});
    one_sided_child.set_minimum_size({1000.0f, 1000.0f});
    one_sided_child.set_anchor(fl7::gui::Anchor::Right);

    auto& layout = container.set_layout<fl7::gui::layout::DefaultLayout>();

    // Width: 10 (left inset) + 40 (right inset) + 100 (filled_child's own
    // minimum width) = 150.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 150.0f );
    // Height: 10 (top inset) + 70 (bottom inset, 100 - (10+20)) + 5
    // (filled_child's own minimum height) = 85.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 85.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  DefaultLayout:  calculate_preferred_size() is, per axis, the largest of every Left+Right (or Top+Bottom) filled child's own required container extent" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& filled_child = container.add_child<fl7::gui::Container>();
    filled_child.set_position({10.0f, 10.0f});
    filled_child.set_size({150.0f, 20.0f}); // 10px left, 40px right.
    filled_child.set_preferred_size({180.0f, 15.0f});
    filled_child.set_anchor(fl7::gui::Anchor::All);

    // Anchored on only one side (Right alone): never actually resizes on
    // this axis at all (see this layout's own doc comment), so it doesn't
    // contribute to calculate_preferred_size() either, regardless of how
    // large its own preferred size is.
    auto& one_sided_child = container.add_child<fl7::gui::Container>();
    one_sided_child.set_position({180.0f, 10.0f});
    one_sided_child.set_size({10.0f, 10.0f});
    one_sided_child.set_preferred_size({1000.0f, 1000.0f});
    one_sided_child.set_anchor(fl7::gui::Anchor::Right);

    auto& layout = container.set_layout<fl7::gui::layout::DefaultLayout>();

    // Width: 10 (left inset) + 40 (right inset) + 180 (filled_child's own
    // preferred width) = 230.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).x, 230.0f );
    // Height: 10 (top inset) + 70 (bottom inset, 100 - (10+20)) + 15
    // (filled_child's own preferred height) = 95.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).y, 95.0f );
}
