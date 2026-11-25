#ifndef FL7_GUI_LAYOUT_GRIDLAYOUT_H
#define FL7_GUI_LAYOUT_GRIDLAYOUT_H
#include "./Layout.h"

#include "../Container.h"
#include "../Face.h"
#include "../Padding.h"

#include <MathLabs/Vector2.h>

#include <cassert>
#include <unordered_map>
#include <vector>



namespace fl7::gui::layout {



/**
 * This layout keeps its own grid of children, independent of the attached
 * container's own child list (which only ever determines Z-order/hit-testing):
 * built up via `add_child()`, which places a new child at an explicit
 * row/column, growing the grid first if either is beyond its current extent
 * (see `get_row_count()`/`get_column_count()`); a child added directly via
 * `Container::add_child()` instead isn't part of this grid, so it's simply left
 * wherever it already is, unarranged by this layout. A cell is either empty or
 * holds exactly one child; `add_child()` asserts that the cell it's given is still
 * empty, rather than silently replacing whatever's already there. A child can also
 * cover more than one row/column (see `set_span()`).
 *
 * Arranges that grid into rows and columns: along the horizontal axis, every column
 * is either exactly as wide as the widest effective preferred width among its own
 * cells (a stretch factor of 0 or less) or a share of whatever space is left over
 * once every such fixed-width column has been accounted for, proportional to its
 * own stretch factor relative to the other stretched columns' (every column's
 * default: every column then just splits the available width evenly, unless some
 * are explicitly pinned to their own widest preferred width via 0 instead), except
 * that a stretched column whose plain share would undercut its own widest effective
 * minimum width instead gets exactly that, with the leftover redistributed among
 * the remaining stretched columns the same way, repeated until none are left
 * undercut; same idea vertically for rows, independently (a stretch factor is kept
 * per row/column here. Every row/column is separated from its neighbors by a fixed
 * row/column gap, within a fixed padding kept around the whole grid.
 *
 * A spanning child doesn't affect the rows/columns it covers until its own
 * preferred/minimum extent there no longer fits within what they already add up to
 * (gaps included). Any shortfall is then split evenly across them. Once sized, such
 * a child's own cell is simply the union of every row/column it covers.
 *
 * How a child actually sits within its own cell (possibly larger than its own
 * preferred size: filled, shrunk-and-aligned near/far, or shrunk-and-centered) is
 * then up to its own anchor, independently per axis; see `BoxLayout`'s own doc
 * comment for the exact rules, identical here.
 *
 * None of this ever shrinks a child past its own effective minimum size, though:
 * once a cell (on either axis) becomes smaller than that, the child keeps its
 * minimum size instead and simply overflows the cell, and possibly this layout's
 * own bounds, rather than shrinking further or distorting its content.
 *
 * Each child's own effective preferred/minimum size is re-queried live on every
 * `perform()` call, not cached here, same as `BoxLayout`.
 *
 * Also reports its own preferred/minimum size: the sum of every row's own tallest
 * preferred/minimum height (plus gaps/padding) by the sum of every column's own
 * widest preferred/minimum width (plus gaps/padding), ignoring stretch/anchor
 * entirely: that's the size this grid would occupy if every child actually got
 * exactly its own row/column's preferred/minimum size, which is what a `Container`
 * with this layout attached reports as its own effective size.
 */
class GridLayout
    : public Layout
{

public:
    /** A child's own row/column span (see `get_span()`/`set_span()`). */
    struct Span
    {
        size_t row_span = 1;
        size_t column_span = 1;
    }; // struct Span



    explicit GridLayout(float row_gap = 0.0f, float column_gap = 0.0f, Padding padding = {})
        : _row_gap(row_gap)
        , _column_gap(column_gap)
        , _padding(padding)
    {
    }

    GridLayout(const GridLayout&) = delete;
    GridLayout& operator=(const GridLayout&) = delete;
    GridLayout(GridLayout&&) = delete;
    GridLayout& operator=(GridLayout&&) = delete;

    ~GridLayout() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the number of rows currently in this grid; see `add_child()`/`set_span()`. */
    size_t get_row_count() const { return _grid.size(); }

    /** Returns the number of columns currently in this grid; see `add_child()`/`set_span()`. */
    size_t get_column_count() const { return _column_count; }

    /** Returns the fixed gap kept between consecutive rows. Default: 0. */
    float get_row_gap() const { return _row_gap; }

    /** Sets the fixed gap kept between consecutive rows. */
    void set_row_gap(float row_gap) { _row_gap = row_gap; }

    /** Returns the fixed gap kept between consecutive columns. Default: 0. */
    float get_column_gap() const { return _column_gap; }

    /** Sets the fixed gap kept between consecutive columns. */
    void set_column_gap(float column_gap) { _column_gap = column_gap; }

    /** Returns the fixed space kept around the whole grid, on every side. Default: none. */
    Padding get_padding() const { return _padding; }

    /** Sets the fixed space kept around the whole grid, independently on each side. */
    void set_padding(Padding padding) { _padding = padding; }

    /** Sets the fixed space kept around the whole grid, the same on every side; see `set_padding(Padding)`. */
    void set_padding(float padding) { set_padding(Padding(padding)); }

    /**
     * Returns the given row's stretch factor; see `set_row_stretch()`.
     * Default: 1 (every row, until told otherwise, participates evenly in
     * distributing the available height).
     */
    float get_row_stretch(size_t row) const;

    /**
     * Sets the given row's stretch factor: 0 or less means its own height is
     * exactly the tallest preferred height among its own cells, not
     * participating in the distribution at all; a positive value (1, the
     * default for every row until told otherwise) means it instead takes
     * that share of whatever height is left over once every non-stretched
     * (stretch <= 0) row has taken its own, relative to the other stretched
     * rows' own factors; same idea as `BoxLayout::set_stretch()`, just per
     * row instead of per child (several cells share a row, so a per-child
     * factor wouldn't be meaningful here). Either way, purely about the
     * row's own height, independent of whether a given cell's own child
     * actually fills it (that's its anchor's call, not this one's; see this
     * class's own doc comment).
     */
    void set_row_stretch(size_t row, float stretch);

    /**
     * Returns the given column's stretch factor; see `set_column_stretch()`.
     * Default: 1.
     */
    float get_column_stretch(size_t column) const;

    /** Sets the given column's stretch factor; see `set_row_stretch()`, same idea along the other axis. */
    void set_column_stretch(size_t column, float stretch);

    /**
     * Returns the given child's own row/column span (see `set_span()`).
     * Default: 1 and 1.
     */
    Span get_span(const Face& child) const;

    /**
     * Grows (or shrinks back) the given child's own footprint to cover the
     * specified number of rows and columns from its own cell, placed earlier via
     * `add_child()`, growing this grid first if that footprint reaches past its
     * current extent. Every cell it would newly cover must still be empty, same as
     * `add_child()` itself requires.
     */
    void set_span(const Face& child, size_t row_span, size_t column_span);



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Constructs a new child Face of the specified type in place, places it
     * at the given row/column of this layout's own grid (growing it first if
     * either is beyond its current extent; see `get_row_count()`/`get_column_count()`),
     * and returns a reference to the new child. That cell must still be empty (see
     * this class's own doc comment). See `set_span()` to grow it past a single
     * row/column afterward.
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& add_child(size_t row, size_t column, Args&&... args)
    {
        if (row >= _grid.size())
            _grid.resize(row + 1);
        if (column >= _column_count)
            _column_count = column + 1;
        for (auto& existing_row : _grid)
            existing_row.resize(_column_count, nullptr);

        assert(_grid[row][column] == nullptr); // Cell already occupied; see this class's own doc comment.

        TFace& ref = _get_container()->add_child<TFace>(std::forward<Args>(args)...);
        _grid[row][column] = &ref;
        perform(*_get_container());
        return ref;
    }



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the
     * rows-and-columns/stretching/anchor-fitting rules described on this class.
     */
    void perform(Container& container) override;

    /**
     * Returns the sum of every row's own tallest effective preferred height (plus
     * the fixed gaps between rows, plus this layout's own padding on that axis) by
     * the sum of every column's own widest effective preferred width (plus the
     * fixed gaps between columns, plus padding there): the size this grid would
     * occupy if every row/column actually got exactly its own preferred extent,
     * regardless of stretch (which only ever redistributes space beyond or below
     * that). A spanning child's own shortfall can grow a row's/column's own
     * contribution here past any single cell's own.
     */
    ml7::Vector2f calculate_preferred_size(const Container& container) const override;

    /**
     * Returns the smallest this grid could ever actually be laid out at without
     * overflowing: the sum, per axis, of every row's/column's own contribution
     * (plus the fixed gaps/padding on that axis), which is either a non-stretched
     * (stretch <= 0) row's/column's own tallest/widest effective preferred extent
     * among its own cells (that's what `perform()` always gives it, never less), or
     * a stretched one's own tallest/widest effective minimum extent instead (the
     * only ones `perform()` can actually shrink that far). A spanning child's own
     * shortfall can grow a row's/column's own contribution here past any single
     * cell's own.
     */
    ml7::Vector2f calculate_minimum_size(const Container& container) const override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    struct TrackExtents
    {
        std::vector<float> row_preferred;
        std::vector<float> row_minimum;
        std::vector<float> column_preferred;
        std::vector<float> column_minimum;
    }; // struct TrackExtents

    /**
     * Every row's own tallest effective preferred/minimum height, and every
     * column's own widest effective preferred/minimum width: first gathered from
     * whichever cells don't themselves span that axis, same as if there were no
     * spans at all; then, for every cell that does span an axis (shortest span
     * first, so shorter ones settle before longer ones build on top of them), grown
     * evenly across its own spanned rows/columns (plus the gaps between them) if
     * its own preferred/minimum extent there doesn't already fit what they add up
     * to. Shared by `perform()`, `calculate_preferred_size()`, and
     * `calculate_minimum_size()`, which otherwise all gather the same thing.
     */
    TrackExtents _measure_tracks() const;

    /**
     * Grows `sizes[start, start + span)` evenly until their sum (plus `gap` kept
     * between them) reaches `own_extent`, or leaves them as they are if it already
     * does. Shared by `_measure_tracks()`'s own row and column passes, otherwise
     * identical.
     */
    static void _grow_spanned_tracks(std::vector<float>& sizes, size_t start, size_t span, float gap, float own_extent);



    // #############################################################################
    // Attributes
    // #############################################################################

    float _row_gap;
    float _column_gap;
    Padding _padding;

    /**
     * This layout's own grid, built up via `add_child()`; see this class's own doc
     * comment. Rows of columns, `nullptr` marking an empty cell; non-owning, each
     * actual Face is one of this layout's own container's children. A spanning
     * child (see `set_span()`) has the same pointer in every cell it covers, not
     * just its own origin. Pruned (cells reset to `nullptr`, not actually shrunk)
     * of any child no longer actually present on every `perform()` call.
     */
    std::vector<std::vector<Face*>> _grid;

    /**
     * This grid's own current column count. Every row in `_grid` is always
     * resized to match it (see `add_child()`/`set_span()`), so this is only
     * ever actually needed on its own with zero rows, where there's no row
     * left to ask.
     */
    size_t _column_count = 0;

    std::unordered_map<size_t, float> _row_stretches;
    std::unordered_map<size_t, float> _column_stretches;

    /** A spanning child's own origin cell and span; see `set_span()`. */
    struct SpanEntry
    {
        size_t row = 0;
        size_t column = 0;
        size_t row_span = 1;
        size_t column_span = 1;
    }; // struct SpanEntry

    /**
     * Every child's own span, keyed by its own address, but only once it's
     * actually larger than 1x1 (see `set_span()`); absent otherwise, same
     * convention as `_row_stretches`/`_column_stretches`'s own default.
     * Pruned the same way as `_grid`.
     */
    std::unordered_map<const Face*, SpanEntry> _spans;

}; // class GridLayout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_GRIDLAYOUT_H
