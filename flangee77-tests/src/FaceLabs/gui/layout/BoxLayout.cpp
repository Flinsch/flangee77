
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

    // set_preferred_size() mirrors set_size() throughout this file: a plain
    // Container has no calculated preferred size of its own (see Face), so
    // whoever wants its explicit size to also be what a layout treats as its
    // preferred size (the common case for a Container used this way) has to
    // say so explicitly; see Face::get_effective_preferred_size().
    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({80.0f, 30.0f});
    child1.set_preferred_size({80.0f, 30.0f});

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({80.0f, 40.0f});
    child2.set_preferred_size({80.0f, 40.0f});

    // Pinned to their own preferred (explicit) size: stretch defaults to 1
    // (every cell splits the main axis evenly), so a plain fixed-size stack
    // needs 0 set explicitly on every child; see BoxLayout's own doc comment.
    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);
    layout.set_stretch(child1, 0.0f);
    layout.set_stretch(child2, 0.0f);
    layout.perform(container);

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

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  two children left at the default stretch (1) split the main axis evenly between their cells, merely repositioned (not resized) since their default anchor doesn't fill" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 5.0f}); // Own (small) height kept: default anchor (Top only) doesn't fill its cell.
    child1.set_preferred_size({50.0f, 5.0f});

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 5.0f});
    child2.set_preferred_size({50.0f, 5.0f});

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 0.0f ); // Its 50-tall cell starts here...
    TESTLABS_CHECK_EQ_FLT( child1.get_size().y, 5.0f ); // ...but it doesn't fill it.
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 50.0f ); // child1's cell (50) + child2's own cell starts right after it.
    TESTLABS_CHECK_EQ_FLT( child2.get_size().y, 5.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  Left+Right anchor fills the cross axis instead of shrinking to the child's own preferred width" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 1000.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_size({80.0f, 30.0f});
    child.set_preferred_size({80.0f, 30.0f});
    child.set_anchor(fl7::gui::Anchor::Left | fl7::gui::Anchor::Right);

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 100.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  equal stretch factors split the leftover main-axis space evenly between stretched children's cells; Top+Bottom anchor fills each cell" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f}); // Own height irrelevant once its cell is stretched and it fills that cell (see anchor below).
    child1.set_preferred_size({50.0f, 20.0f});
    child1.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});
    child2.set_preferred_size({50.0f, 20.0f});
    child2.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    layout.set_stretch(child1, 1.0f);
    layout.set_stretch(child2, 1.0f);
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( child1.get_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a stretched child's cell takes whatever space is left over after every non-stretched (stretch 0, the default) sibling's cell has taken its own preferred size; Top+Bottom anchor fills that cell" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& fixed_child = container.add_child<fl7::gui::Container>();
    fixed_child.set_size({50.0f, 20.0f}); // Its cell is exactly this.
    fixed_child.set_preferred_size({50.0f, 20.0f});

    auto& stretched_child = container.add_child<fl7::gui::Container>();
    stretched_child.set_size({50.0f, 10.0f}); // Own height overridden by filling its stretched cell.
    stretched_child.set_preferred_size({50.0f, 10.0f});
    stretched_child.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    layout.set_stretch(fixed_child, 0.0f); // Pinned; stretch defaults to 1, see BoxLayout's own doc comment.
    layout.set_stretch(stretched_child, 1.0f); // Redundant (already the default), but explicit for clarity.
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( fixed_child.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( fixed_child.get_size().y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( stretched_child.get_position().y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( stretched_child.get_size().y, 80.0f ); // 100 - 20.
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  stretch and anchor are independent: a stretched child whose anchor doesn't fill the main axis keeps its own preferred size, merely positioned within its own (larger) cell" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& fixed_child = container.add_child<fl7::gui::Container>();
    fixed_child.set_size({50.0f, 20.0f});
    fixed_child.set_preferred_size({50.0f, 20.0f});

    // Stretched (gets a 80-tall cell, same as the previous test), but with
    // the default anchor (Top only, no Bottom): doesn't fill that cell, just
    // keeps its own 10-tall preferred size, aligned to the cell's own top.
    auto& stretched_child = container.add_child<fl7::gui::Container>();
    stretched_child.set_size({50.0f, 10.0f});
    stretched_child.set_preferred_size({50.0f, 10.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    layout.set_stretch(fixed_child, 0.0f); // Pinned; stretch defaults to 1, see BoxLayout's own doc comment.
    layout.set_stretch(stretched_child, 1.0f); // Redundant (already the default), but explicit for clarity.
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( stretched_child.get_position().y, 20.0f ); // Its cell still starts here...
    TESTLABS_CHECK_EQ_FLT( stretched_child.get_size().y, 10.0f ); // ...but it doesn't grow to fill it (80).
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a child's cross-axis size recovers to its own original preferred size after the container shrinks and then grows back" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 1000.0f});

    auto& child = container.add_child<fl7::gui::Container>();
    child.set_size({150.0f, 30.0f});
    child.set_preferred_size({150.0f, 30.0f}); // Cross-axis (x) preferred size: 150.

    container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 150.0f ); // Unchanged: already smaller than the 200-wide container.

    // Shrinks the container below the child's own preferred width: the
    // child's cross-axis size is forced down to fill it instead (there's
    // no narrower "preferred" to fall back to).
    container.set_size({100.0f, 1000.0f});
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 100.0f );

    // Grows the container back past the child's own ORIGINAL preferred
    // width (150): without caching that original value (see BoxLayout's
    // own doc comment), this would incorrectly stay stuck at 100.
    container.set_size({200.0f, 1000.0f});
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 150.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  padding keeps a fixed space around the whole sequence, on top of the gap between children" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& child1 = container.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f});
    child1.set_preferred_size({50.0f, 20.0f});

    auto& child2 = container.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});
    child2.set_preferred_size({50.0f, 20.0f});

    // Pinned to their own preferred (explicit) size; stretch defaults to 1,
    // see BoxLayout's own doc comment.
    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);
    layout.set_stretch(child1, 0.0f);
    layout.set_stretch(child2, 0.0f);
    layout.set_padding(5.0f);
    layout.perform(container);

    // Main axis: starts at the top padding (5), then sequenced as usual.
    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 5.0f );
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 35.0f ); // 5 + 20 + 10 (gap).

    // Cross axis: left-aligned (default anchor) within the padded area, not at 0.
    TESTLABS_CHECK_EQ_FLT( child1.get_position().x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( child1.get_size().x, 50.0f );
}
