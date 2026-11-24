
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/layout/BoxLayout.h>



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a vertical box sequences children by their own (explicit) height plus a fixed gap, left-aligned (default anchor) on the cross axis" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 1000.0f});

    // Pinned to their own preferred (explicit) size: stretch defaults to 1
    // (every cell splits the main axis evenly), so a plain fixed-size stack
    // needs 0 set explicitly on every child; see BoxLayout's own doc comment.
    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);

    // set_preferred_size() mirrors set_size() throughout this file: a plain
    // Container has no calculated preferred size of its own (see Face), so
    // whoever wants its explicit size to also be what a layout treats as its
    // preferred size (the common case for a Container used this way) has to
    // say so explicitly; see Face::get_effective_preferred_size().
    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_size({80.0f, 30.0f});
    child1.set_preferred_size({80.0f, 30.0f});

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_size({80.0f, 40.0f});
    child2.set_preferred_size({80.0f, 40.0f});

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

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 5.0f}); // Own (small) height kept: default anchor (Top only) doesn't fill its cell.
    child1.set_preferred_size({50.0f, 5.0f});

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 5.0f});
    child2.set_preferred_size({50.0f, 5.0f});

    // Needed: add_child() above already (re-)performed this layout once per
    // call, but before the child it just added was actually configured.
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( child1.get_position().y, 0.0f ); // Its 50-tall cell starts here...
    TESTLABS_CHECK_EQ_FLT( child1.get_size().y, 5.0f ); // ...but it doesn't fill it.
    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 50.0f ); // child1's cell (50) + child2's own cell starts right after it.
    TESTLABS_CHECK_EQ_FLT( child2.get_size().y, 5.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  Left+Right anchor fills the cross axis instead of shrinking to the child's own preferred width" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 1000.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& child = layout.add_child<fl7::gui::Container>();
    child.set_size({80.0f, 30.0f});
    child.set_preferred_size({80.0f, 30.0f});
    child.set_anchor(fl7::gui::Anchor::Left | fl7::gui::Anchor::Right);

    layout.perform(container); // Needed: see the previous test's own comment.

    TESTLABS_CHECK_EQ_FLT( child.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 100.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  equal stretch factors split the leftover main-axis space evenly between stretched children's cells; Top+Bottom anchor fills each cell" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f}); // Own height irrelevant once its cell is stretched and it fills that cell (see anchor below).
    child1.set_preferred_size({50.0f, 20.0f});
    child1.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});
    child2.set_preferred_size({50.0f, 20.0f});
    child2.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

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

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& fixed_child = layout.add_child<fl7::gui::Container>();
    fixed_child.set_size({50.0f, 20.0f}); // Its cell is exactly this.
    fixed_child.set_preferred_size({50.0f, 20.0f});

    auto& stretched_child = layout.add_child<fl7::gui::Container>();
    stretched_child.set_size({50.0f, 10.0f}); // Own height overridden by filling its stretched cell.
    stretched_child.set_preferred_size({50.0f, 10.0f});
    stretched_child.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom);

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

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& fixed_child = layout.add_child<fl7::gui::Container>();
    fixed_child.set_size({50.0f, 20.0f});
    fixed_child.set_preferred_size({50.0f, 20.0f});

    // Stretched (gets a 80-tall cell, same as the previous test), but with
    // the default anchor (Top only, no Bottom): doesn't fill that cell, just
    // keeps its own 10-tall preferred size, aligned to the cell's own top.
    auto& stretched_child = layout.add_child<fl7::gui::Container>();
    stretched_child.set_size({50.0f, 10.0f});
    stretched_child.set_preferred_size({50.0f, 10.0f});

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

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& child = layout.add_child<fl7::gui::Container>();
    child.set_size({150.0f, 30.0f});
    child.set_preferred_size({150.0f, 30.0f}); // Cross-axis (x) preferred size: 150.

    layout.perform(container); // Needed: add_child() above ran before the child was configured.
    TESTLABS_CHECK_EQ_FLT( child.get_size().x, 150.0f ); // Unchanged: already smaller than the 200-wide container.

    // Shrinks the container below the child's own preferred width: the
    // child's cross-axis size is forced down to fill it instead (there's
    // no narrower "preferred" to fall back to). Container::set_size() itself
    // re-performs this layout automatically, see Container::_on_size_changed().
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

    // Pinned to their own preferred (explicit) size; stretch defaults to 1,
    // see BoxLayout's own doc comment.
    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f);

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_size({50.0f, 20.0f});
    child1.set_preferred_size({50.0f, 20.0f});

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_size({50.0f, 20.0f});
    child2.set_preferred_size({50.0f, 20.0f});

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

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  insert_child() places a new child at an explicit index within this layout's own sequence, instead of always at the end" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& first = layout.add_child<fl7::gui::Container>();
    first.set_preferred_size({50.0f, 10.0f});
    layout.set_stretch(first, 0.0f);

    auto& last = layout.add_child<fl7::gui::Container>();
    last.set_preferred_size({50.0f, 10.0f});
    layout.set_stretch(last, 0.0f);

    // Inserted between the other two, not appended after "last".
    auto& middle = layout.insert_child<fl7::gui::Container>(1);
    middle.set_preferred_size({50.0f, 10.0f});
    layout.set_stretch(middle, 0.0f);
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( first.get_position().y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( middle.get_position().y, 10.0f );
    TESTLABS_CHECK_EQ_FLT( last.get_position().y, 20.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a child added directly via Container::add_child() instead of this layout's own add_child()/insert_child() isn't part of its sequence, so it's left wherever it already is" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& bypassed = container.add_child<fl7::gui::Container>();
    bypassed.set_position({42.0f, 42.0f});
    bypassed.set_size({10.0f, 10.0f});
    bypassed.set_preferred_size({10.0f, 10.0f});
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( bypassed.get_position().x, 42.0f );
    TESTLABS_CHECK_EQ_FLT( bypassed.get_position().y, 42.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a child removed from the container is pruned from this layout's own sequence (and any stretch override), instead of leaving a dangling entry behind" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical);

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_preferred_size({50.0f, 10.0f});
    layout.set_stretch(child1, 0.0f);

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_preferred_size({50.0f, 10.0f});
    layout.set_stretch(child2, 0.0f);

    container.remove_child(child1); // Also re-performs this layout, pruning it on the way; see Container::remove_child().

    TESTLABS_CHECK_EQ_FLT( child2.get_position().y, 0.0f ); // Took over child1's former spot at the front.
}



// #############################################################################
// calculate_preferred_size()/calculate_minimum_size()
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  calculate_preferred_size()/calculate_minimum_size() with no children are just this layout's own padding, on both axes" )
{
    fl7::gui::Container container;

    fl7::gui::layout::BoxLayout layout(fl7::gui::layout::BoxLayout::Orientation::Horizontal, 10.0f, fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f));

    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).x, 8.0f ); // left(4) + right(4); no gap without a second child.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).y, 4.0f ); // top(2) + bottom(2).
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 8.0f );
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 4.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  calculate_preferred_size() sums every child's own preferred size along the main axis (plus gaps/padding), and takes their max along the cross axis (plus padding), regardless of stretch or anchor" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Vertical, 10.0f, fl7::gui::Padding(5.0f));

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_preferred_size({50.0f, 20.0f});

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_preferred_size({30.0f, 35.0f}); // taller than child1: wins the cross-axis max.
    child2.set_anchor(fl7::gui::Anchor::Top | fl7::gui::Anchor::Bottom); // stretch/anchor: irrelevant to this calculation either way.

    layout.set_stretch(child2, 2.0f);

    // Main axis (y): padding top+bottom (10) + gap (10) + both children's own
    // preferred heights (20 + 35) = 75.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).y, 75.0f );
    // Cross axis (x): padding left+right (10) + the wider child's own preferred
    // width (50, not 30): child2's own 35 is its HEIGHT, not relevant here.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).x, 60.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  calculate_minimum_size() is calculate_preferred_size()'s exact counterpart, against every child's own effective minimum size instead" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal, 10.0f);

    auto& child1 = layout.add_child<fl7::gui::Container>();
    child1.set_preferred_size({50.0f, 20.0f});
    child1.set_minimum_size({10.0f, 5.0f});

    auto& child2 = layout.add_child<fl7::gui::Container>();
    child2.set_preferred_size({30.0f, 35.0f});
    child2.set_minimum_size({15.0f, 8.0f});

    // Main axis (x): no padding, one gap (10), both children's own minimum
    // widths (10 + 15) = 35. (Their much larger preferred widths, 50/30, don't
    // factor in at all here.)
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 35.0f );
    // Cross axis (y): the larger of their own minimum heights (8, not 5).
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 8.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  calculate_minimum_size() uses a non-stretched (stretch <= 0) child's own preferred size along the main axis instead, not its own (possibly smaller) minimum size, since perform() never actually shrinks it that far" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal, 10.0f);

    auto& fixed_child = layout.add_child<fl7::gui::Container>();
    fixed_child.set_preferred_size({50.0f, 20.0f});
    fixed_child.set_minimum_size({10.0f, 5.0f}); // Much smaller, but never actually reached; see below.
    layout.set_stretch(fixed_child, 0.0f);

    auto& stretched_child = layout.add_child<fl7::gui::Container>();
    stretched_child.set_preferred_size({30.0f, 35.0f});
    stretched_child.set_minimum_size({15.0f, 8.0f});
    // Left at the default stretch (1): unlike fixed_child above, this one
    // actually can shrink all the way to its own minimum once space runs short.

    // Main axis (x): one gap (10) + fixed_child's own preferred width (50, not
    // its smaller minimum 10) + stretched_child's own minimum width (15) = 75.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 75.0f );
    // Cross axis (y): the larger of their own minimum heights (8, not 5), same
    // as before: no stretch distinction along the cross axis.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 8.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  Container::get_effective_preferred_size()/get_effective_minimum_size() defer to this layout's own calculation while it's attached, end to end" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal, 0.0f, fl7::gui::Padding(2.0f));

    auto& child = layout.add_child<fl7::gui::Container>();
    child.set_position({100.0f, 100.0f}); // Deliberately irrelevant: BoxLayout ignores a child's own position here.
    child.set_preferred_size({20.0f, 30.0f});
    child.set_minimum_size({5.0f, 8.0f});

    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 24.0f ); // 20 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 34.0f ); // 30 + top(2) + bottom(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 9.0f ); // 5 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 12.0f ); // 8 + top(2) + bottom(2).
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  BoxLayout:  a Container's own Style::padding stops applying once this layout is attached (see Face::_applies_own_padding()), so it doesn't double up on top of this layout's own, separate get_padding()" )
{
    fl7::gui::Container container;

    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(50.0f); // Deliberately huge: would obviously show up below if it applied at all.
    container.set_style_override(style);

    auto& layout = container.set_layout<fl7::gui::layout::BoxLayout>(fl7::gui::layout::BoxLayout::Orientation::Horizontal, 0.0f, fl7::gui::Padding(2.0f));

    auto& child = layout.add_child<fl7::gui::Container>();
    child.set_preferred_size({20.0f, 30.0f});

    // Only this layout's own 2px padding shows up, not the container's own 50px on top of it.
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 24.0f ); // 20 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 34.0f ); // 30 + top(2) + bottom(2).

    // Nor does the container's own 50px padding shift the child (see
    // Container::get_scroll_offset()): also suppressed while this layout is attached.
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().y, 0.0f );
}
