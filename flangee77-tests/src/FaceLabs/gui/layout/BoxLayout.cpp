
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/layout/BoxLayout.h>



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a vertical box sequences children by their own (explicit) height plus a fixed gap, left-aligned (default anchor) on the cross axis" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 1000.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({80.0f, 30.0f});

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({80.0f, 40.0f});

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);

    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_size().y, 30.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 40.0f ); // 30 (child1) + 10 (gap).
    TESTLABS_CHECK_EQ_FLT( child2.get_size().y, 40.0f );

    // Cross axis (width): neither child's own 80 matches the container's 100,
    // so both stay at their own (preferred) width, aligned left (Left is the
    // only cross-axis anchor flag set by default, see Face::get_anchor()).
    TESTLABS_CHECK_EQ_FLT( child1.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_size().x, 80.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  Left+Right anchor fills the cross axis instead of shrinking to the child's own preferred width" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 1000.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_size({80.0f, 30.0f});
    child.set_anchor(fl7::gui::Anchor::Left | fl7::gui::Anchor::Right);

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 100.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  equal stretch factors split the leftover main-axis space evenly between stretched children" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f}); // Own height irrelevant once stretched.

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    layout.set_stretch(child1, 1.0f);
    layout.set_stretch(child2, 1.0f);
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( child1.get_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a stretched child takes whatever space is left over after every non-stretched (stretch 0, the default) sibling has taken its own preferred size" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& fixed_child = container.add_child<fl7::gui::Container>();
    fixed_child.set_size({50.0f, 20.0f}); // Not stretched (default): takes exactly this.

    auto& stretched_child = container.add_child<fl7::gui::Container>();
    stretched_child.set_size({50.0f, 10.0f}); // Own height overridden by stretching.

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    layout.set_stretch(stretched_child, 1.0f);
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( fixed_child.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( fixed_child.get_size().y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( stretched_child.get_position().y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( stretched_child.get_size().y, 80.0f ); // 100 - 20.
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  padding keeps a fixed space around the whole sequence, on top of the gap between children" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f});

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);
    layout.set_padding(5.0f);
    layout.perform(container);

    // Main axis: starts at the top padding (5), then sequenced as usual.
    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 5.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 35.0f ); // 5 + 20 + 10 (gap).

    // Cross axis: left-aligned (default anchor) within the padded area, not at 0.
    TESTLABS_CHECK_EQ_FLT( child1.get_position().x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_size().x, 50.0f );
}
