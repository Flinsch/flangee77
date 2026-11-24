
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Style.h>
#include <FaceLabs/gui/layout/GridLayout.h>



// #############################################################################
// Plain API (no Shell/input involved)
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  a 2x2 grid sizes each row/column by the tallest/widest of its own cells, left-aligned (default anchor) within each" )
{
    fl7::gui::Container container;
    container.set_size({200.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();
    layout.set_row_stretch(0, 0.0f);
    layout.set_row_stretch(1, 0.0f);
    layout.set_column_stretch(0, 0.0f);
    layout.set_column_stretch(1, 0.0f);

    auto& top_left = layout.add_child<fl7::gui::Container>(0, 0);
    top_left.set_preferred_size({50.0f, 20.0f});

    auto& top_right = layout.add_child<fl7::gui::Container>(0, 1);
    top_right.set_preferred_size({80.0f, 20.0f}); // widest in column 1.

    auto& bottom_left = layout.add_child<fl7::gui::Container>(1, 0);
    bottom_left.set_preferred_size({50.0f, 30.0f}); // tallest in row 1.

    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( top_left.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( top_left.get_position().y, 0.0f );
    // Column 0's own width (50, the wider of top_left's/bottom_left's own 50).
    TESTLABS_CHECK_EQ_FLT( top_right.get_position().x, 50.0f );
    // Row 0's own height (20, the taller of top_left's/top_right's own 20).
    TESTLABS_CHECK_EQ_FLT( bottom_left.get_position().y, 20.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  equal row/column stretch factors split the leftover space evenly between their own cells; Left+Right+Top+Bottom anchor fills each cell" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();

    auto& top_left = layout.add_child<fl7::gui::Container>(0, 0);
    auto& top_right = layout.add_child<fl7::gui::Container>(0, 1);
    auto& bottom_left = layout.add_child<fl7::gui::Container>(1, 0);
    auto& bottom_right = layout.add_child<fl7::gui::Container>(1, 1);
    for (auto* cell : {&top_left, &top_right, &bottom_left, &bottom_right})
    {
        cell->set_preferred_size({10.0f, 10.0f});
        cell->set_anchor(fl7::gui::Anchor::All);
    }
    layout.perform(container);

    // Default stretch (1) everywhere: splits both axes evenly into two 50-wide/tall cells.
    TESTLABS_CHECK_EQ_FLT( top_left.get_size().x, 50.0f );
    TESTLABS_CHECK_EQ_FLT( top_left.get_size().y, 50.0f );
    TESTLABS_CHECK_EQ_FLT( bottom_right.get_position().x, 50.0f );
    TESTLABS_CHECK_EQ_FLT( bottom_right.get_position().y, 50.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  stretch and anchor are independent: a stretched row/column whose cell's anchor doesn't fill it keeps the cell's own preferred size, merely positioned within the larger row/column" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();

    auto& cell = layout.add_child<fl7::gui::Container>(0, 0);
    cell.set_preferred_size({10.0f, 10.0f}); // Default anchor (Left|Top): doesn't fill its (stretched) cell.
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( cell.get_position().x, 0.0f ); // Its cell still starts here...
    TESTLABS_CHECK_EQ_FLT( cell.get_size().x, 10.0f ); // ...but it doesn't grow to fill it (100, the only column's own stretched width).
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  a cell never shrinks past its own effective minimum size, even once its row/column has" )
{
    fl7::gui::Container container;
    container.set_size({20.0f, 20.0f}); // Much smaller than the cell's own preferred size.

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();

    auto& cell = layout.add_child<fl7::gui::Container>(0, 0);
    cell.set_preferred_size({50.0f, 50.0f});
    cell.set_minimum_size({30.0f, 30.0f});
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( cell.get_size().x, 30.0f ); // Floored at its own minimum, overflowing the container.
    TESTLABS_CHECK_EQ_FLT( cell.get_size().y, 30.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  row_gap/column_gap and padding keep fixed space around and between rows/columns, independently per axis" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>(10.0f, 5.0f, fl7::gui::Padding(2.0f));
    layout.set_row_stretch(0, 0.0f);
    layout.set_row_stretch(1, 0.0f);
    layout.set_column_stretch(0, 0.0f);
    layout.set_column_stretch(1, 0.0f);

    auto& top_left = layout.add_child<fl7::gui::Container>(0, 0);
    top_left.set_preferred_size({20.0f, 20.0f});
    auto& top_right = layout.add_child<fl7::gui::Container>(0, 1);
    top_right.set_preferred_size({20.0f, 20.0f});
    auto& bottom_left = layout.add_child<fl7::gui::Container>(1, 0);
    bottom_left.set_preferred_size({20.0f, 20.0f});
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( top_left.get_position().x, 2.0f ); // left padding.
    TESTLABS_CHECK_EQ_FLT( top_left.get_position().y, 2.0f ); // top padding.
    TESTLABS_CHECK_EQ_FLT( top_right.get_position().x, 27.0f ); // 2 + 20 (column 0) + 5 (column_gap).
    TESTLABS_CHECK_EQ_FLT( bottom_left.get_position().y, 32.0f ); // 2 + 20 (row 0) + 10 (row_gap).
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  add_child() grows this layout's own grid to fit whatever row/column it's given, reporting that via get_row_count()/get_column_count()" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();
    TESTLABS_CHECK_EQ( layout.get_row_count(), 0u );
    TESTLABS_CHECK_EQ( layout.get_column_count(), 0u );

    layout.add_child<fl7::gui::Container>(2, 3);

    TESTLABS_CHECK_EQ( layout.get_row_count(), 3u ); // Rows 0-2.
    TESTLABS_CHECK_EQ( layout.get_column_count(), 4u ); // Columns 0-3.
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  a child added directly via Container::add_child() instead of this layout's own add_child() isn't part of its grid, so it's left wherever it already is" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();

    auto& bypassed = container.add_child<fl7::gui::Container>();
    bypassed.set_position({42.0f, 42.0f});
    bypassed.set_size({10.0f, 10.0f});
    bypassed.set_preferred_size({10.0f, 10.0f});
    layout.perform(container);

    TESTLABS_CHECK_EQ_FLT( bypassed.get_position().x, 42.0f );
    TESTLABS_CHECK_EQ_FLT( bypassed.get_position().y, 42.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  a child removed from the container frees its own cell again, instead of leaving a dangling entry behind" )
{
    fl7::gui::Container container;
    container.set_size({100.0f, 100.0f});

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>();

    auto& first = layout.add_child<fl7::gui::Container>(0, 0);
    first.set_preferred_size({10.0f, 10.0f});

    container.remove_child(first); // Also re-performs this layout, freeing its cell on the way.

    // Would assert (cell still occupied) if the removal above hadn't actually
    // freed it back up.
    auto& second = layout.add_child<fl7::gui::Container>(0, 0);
    second.set_preferred_size({10.0f, 10.0f});

    TESTLABS_CHECK_EQ_FLT( second.get_position().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( second.get_position().y, 0.0f );
}



// #############################################################################
// calculate_preferred_size()/calculate_minimum_size()
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  calculate_preferred_size()/calculate_minimum_size() with no children are just this layout's own padding, on both axes" )
{
    fl7::gui::Container container;

    fl7::gui::layout::GridLayout layout(10.0f, 5.0f, fl7::gui::Padding(4.0f, 2.0f, 4.0f, 2.0f));

    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).x, 8.0f ); // left(4) + right(4).
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).y, 4.0f ); // top(2) + bottom(2).
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 8.0f );
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 4.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  calculate_preferred_size() sums every row's own tallest preferred height (plus gaps/padding) by every column's own widest preferred width (plus gaps/padding), regardless of stretch or anchor" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>(10.0f, 5.0f, fl7::gui::Padding(2.0f));

    auto& top_left = layout.add_child<fl7::gui::Container>(0, 0);
    top_left.set_preferred_size({20.0f, 10.0f});

    auto& top_right = layout.add_child<fl7::gui::Container>(0, 1);
    top_right.set_preferred_size({30.0f, 10.0f}); // Wider (not taller): wins column 1's own max, but not row 0's.
    top_right.set_anchor(fl7::gui::Anchor::All); // Stretch/anchor: irrelevant to this calculation either way.

    auto& bottom_left = layout.add_child<fl7::gui::Container>(1, 0);
    bottom_left.set_preferred_size({25.0f, 40.0f}); // Taller: wins row 1's own max.

    // Row heights: 10 (row 0) + 40 (row 1) + row_gap(10) + padding(4) = 64.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).y, 64.0f );
    // Column widths: 25 (column 0, from bottom_left, wider than top_left's 20)
    // + 30 (column 1) + column_gap(5) + padding(4) = 64.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_preferred_size(container).x, 64.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  calculate_minimum_size() uses a non-stretched (stretch <= 0) row's/column's own tallest/widest preferred extent instead, not its own (possibly smaller) minimum extent, since perform() never actually shrinks it that far" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>(10.0f, 5.0f, fl7::gui::Padding(2.0f));
    layout.set_row_stretch(0, 0.0f);
    layout.set_column_stretch(0, 0.0f);
    // Row 1/column 1 left at the default stretch (1): unlike row 0/column 0
    // above, these actually can shrink all the way to their own minimum once
    // space runs short.

    auto& top_left = layout.add_child<fl7::gui::Container>(0, 0); // Fixed row, fixed column.
    top_left.set_preferred_size({50.0f, 20.0f});
    top_left.set_minimum_size({10.0f, 5.0f}); // Much smaller, but never actually reached; see below.

    auto& top_right = layout.add_child<fl7::gui::Container>(0, 1); // Fixed row, stretched column.
    top_right.set_preferred_size({30.0f, 18.0f});
    top_right.set_minimum_size({15.0f, 8.0f});

    auto& bottom_left = layout.add_child<fl7::gui::Container>(1, 0); // Stretched row, fixed column.
    bottom_left.set_preferred_size({45.0f, 35.0f});
    bottom_left.set_minimum_size({10.0f, 8.0f});

    auto& bottom_right = layout.add_child<fl7::gui::Container>(1, 1); // Stretched row, stretched column.
    bottom_right.set_preferred_size({30.0f, 35.0f});
    bottom_right.set_minimum_size({15.0f, 8.0f});

    // Row heights: row 0 (fixed) is its own tallest preferred height (20, not
    // either cell's smaller minimum), row 1 (stretched) is its own tallest
    // minimum height (8) instead; + row_gap(10) + padding(4) = 42.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).y, 42.0f );
    // Column widths: column 0 (fixed) is its own widest preferred width (50),
    // column 1 (stretched) is its own widest minimum width (15) instead;
    // + column_gap(5) + padding(4) = 74.
    TESTLABS_CHECK_EQ_FLT( layout.calculate_minimum_size(container).x, 74.0f );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  Container::get_effective_preferred_size()/get_effective_minimum_size() defer to this layout's own calculation while it's attached, end to end" )
{
    fl7::gui::Container container;

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>(0.0f, 0.0f, fl7::gui::Padding(2.0f));

    auto& cell = layout.add_child<fl7::gui::Container>(0, 0);
    cell.set_position({100.0f, 100.0f}); // Deliberately irrelevant: GridLayout ignores a cell's own position here.
    cell.set_preferred_size({20.0f, 30.0f});
    cell.set_minimum_size({5.0f, 8.0f});

    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 24.0f ); // 20 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 34.0f ); // 30 + top(2) + bottom(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().x, 9.0f ); // 5 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_minimum_size().y, 12.0f ); // 8 + top(2) + bottom(2).
}

TESTLABS_CASE( u8"FaceLabs:  gui:  layout:  GridLayout:  a Container's own Style::padding stops applying once this layout is attached (see Face::_applies_own_padding()), so it doesn't double up on top of this layout's own, separate get_padding()" )
{
    fl7::gui::Container container;

    fl7::gui::Style style;
    style.padding = fl7::gui::Padding(50.0f); // Deliberately huge: would obviously show up below if it applied at all.
    container.set_style_override(style);

    auto& layout = container.set_layout<fl7::gui::layout::GridLayout>(0.0f, 0.0f, fl7::gui::Padding(2.0f));

    auto& cell = layout.add_child<fl7::gui::Container>(0, 0);
    cell.set_preferred_size({20.0f, 30.0f});

    // Only this layout's own 2px padding shows up, not the container's own 50px on top of it.
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().x, 24.0f ); // 20 + left(2) + right(2).
    TESTLABS_CHECK_EQ_FLT( container.get_effective_preferred_size().y, 34.0f ); // 30 + top(2) + bottom(2).

    // Nor does the container's own 50px padding shift the cell (see
    // Container::get_scroll_offset()): also suppressed while this layout is attached.
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( container.get_scroll_offset().y, 0.0f );
}
