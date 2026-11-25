#include "GridLayout.h"

#include "../Container.h"
#include "../Face.h"

#include <algorithm>
#include <unordered_set>



namespace fl7::gui::layout {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the given row's stretch factor; see `set_row_stretch()`.
     * Default: 1 (every row, until told otherwise, participates evenly in
     * distributing the available height).
     */
    float GridLayout::get_row_stretch(size_t row) const
    {
        const auto it = _row_stretches.find(row);
        return it != _row_stretches.end() ? it->second : 1.0f;
    }

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
    void GridLayout::set_row_stretch(size_t row, float stretch)
    {
        _row_stretches[row] = stretch;
    }

    /**
     * Returns the given column's stretch factor; see `set_column_stretch()`.
     * Default: 1.
     */
    float GridLayout::get_column_stretch(size_t column) const
    {
        const auto it = _column_stretches.find(column);
        return it != _column_stretches.end() ? it->second : 1.0f;
    }

    /** Sets the given column's stretch factor; see `set_row_stretch()`, same idea along the other axis. */
    void GridLayout::set_column_stretch(size_t column, float stretch)
    {
        _column_stretches[column] = stretch;
    }

    /**
     * Returns the given child's own row/column span (see `set_span()`).
     * Default: 1 and 1.
     */
    GridLayout::Span GridLayout::get_span(const Face& child) const
    {
        const auto it = _spans.find(&child);
        return it != _spans.end() ? Span{it->second.row_span, it->second.column_span} : Span{};
    }

    /**
     * Grows (or shrinks back) the given child's own footprint to cover the
     * specified number of rows and columns from its own cell, placed earlier via
     * `add_child()`, growing this grid first if that footprint reaches past its
     * current extent. Every cell it would newly cover must still be empty, same as
     * `add_child()` itself requires.
     */
    void GridLayout::set_span(const Face& child, size_t row_span, size_t column_span)
    {
        assert(row_span >= 1 && column_span >= 1);

        size_t origin_row = 0;
        size_t origin_column = 0;
        size_t old_row_span = 1;
        size_t old_column_span = 1;

        const auto span_it = _spans.find(&child);
        if (span_it != _spans.end())
        {
            origin_row = span_it->second.row;
            origin_column = span_it->second.column;
            old_row_span = span_it->second.row_span;
            old_column_span = span_it->second.column_span;
        }
        else
        {
            bool found = false;
            for (size_t r = 0; r < _grid.size() && !found; ++r)
            {
                for (size_t c = 0; c < _column_count && !found; ++c)
                {
                    if (_grid[r][c] == &child)
                    {
                        origin_row = r;
                        origin_column = c;
                        found = true;
                    }
                }
            }
            assert(found); // Child not placed via this layout's own add_child() yet.
        }

        Face* const child_ptr = _grid[origin_row][origin_column];

        for (size_t r = origin_row; r < origin_row + old_row_span; ++r)
            for (size_t c = origin_column; c < origin_column + old_column_span; ++c)
                if (_grid[r][c] == child_ptr)
                    _grid[r][c] = nullptr;

        const size_t needed_row_count = origin_row + row_span;
        const size_t needed_column_count = origin_column + column_span;
        if (needed_row_count > _grid.size())
            _grid.resize(needed_row_count);
        if (needed_column_count > _column_count)
            _column_count = needed_column_count;
        for (auto& existing_row : _grid)
            existing_row.resize(_column_count, nullptr);

        for (size_t r = origin_row; r < origin_row + row_span; ++r)
            for (size_t c = origin_column; c < origin_column + column_span; ++c)
                assert(_grid[r][c] == nullptr); // Cell already occupied; see this class's own doc comment.

        for (size_t r = origin_row; r < origin_row + row_span; ++r)
            for (size_t c = origin_column; c < origin_column + column_span; ++c)
                _grid[r][c] = child_ptr;

        if (row_span == 1 && column_span == 1)
            _spans.erase(&child);
        else
            _spans[&child] = {origin_row, origin_column, row_span, column_span};

        perform(*_get_container());
    }



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the
     * rows-and-columns/stretching/anchor-fitting rules described on this class.
     */
    void GridLayout::perform(Container& container)
    {
        // Prunes _grid of any child no longer actually present (cells reset
        // to nullptr, not actually shrunk; see this class's own doc comment).
        std::unordered_set<const Face*> current;
        for (const auto& child : container.get_children())
            current.insert(child.get());
        for (auto& row : _grid)
            for (Face*& cell : row)
                if (cell && !current.contains(cell))
                    cell = nullptr;
        for (auto it = _spans.begin(); it != _spans.end(); )
            it = current.contains(it->first) ? std::next(it) : _spans.erase(it);

        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        if (row_count == 0 || column_count == 0)
            return;

        // Every row's own tallest preferred/minimum height, every column's
        // own widest preferred/minimum width, spans included.
        const auto [row_preferred, row_minimum, column_preferred, column_minimum] = _measure_tracks();

        const ml7::Vector2f container_size = container.get_size();
        const float total_row_gap = _row_gap * static_cast<float>(row_count - 1);
        const float total_column_gap = _column_gap * static_cast<float>(column_count - 1);

        // Distributes the available height/width across stretched rows/columns,
        // same idea as BoxLayout::perform(), just independently per axis.
        float row_fixed_total = 0.0f;
        float row_stretch_sum = 0.0f;
        std::vector<float> row_stretches;
        std::vector<float> row_stretch_minimums;
        for (size_t r = 0; r < row_count; ++r)
        {
            const float stretch = get_row_stretch(r);
            if (stretch > 0.0f)
            {
                row_stretch_sum += stretch;
                row_stretches.push_back(stretch);
                row_stretch_minimums.push_back(row_minimum[r]);
            }
            else
            {
                row_fixed_total += row_preferred[r];
            }
        }

        float column_fixed_total = 0.0f;
        float column_stretch_sum = 0.0f;
        std::vector<float> column_stretches;
        std::vector<float> column_stretch_minimums;
        for (size_t c = 0; c < column_count; ++c)
        {
            const float stretch = get_column_stretch(c);
            if (stretch > 0.0f)
            {
                column_stretch_sum += stretch;
                column_stretches.push_back(stretch);
                column_stretch_minimums.push_back(column_minimum[c]);
            }
            else
            {
                column_fixed_total += column_preferred[c];
            }
        }

        const float row_available = std::max(0.0f, container_size.y - _padding.top - _padding.bottom - total_row_gap - row_fixed_total);
        const float column_available = std::max(0.0f, container_size.x - _padding.left - _padding.right - total_column_gap - column_fixed_total);

        // Every stretched row's/column's own final size, honoring its own
        // minimum (see _distribute_stretched_space()'s own doc comment), in
        // the same order as encountered above/below.
        const std::vector<float> row_stretched_sizes = _distribute_stretched_space(row_available, row_stretches, row_stretch_minimums);
        const std::vector<float> column_stretched_sizes = _distribute_stretched_space(column_available, column_stretches, column_stretch_minimums);

        std::vector<float> row_size(row_count);
        std::vector<float> row_offset(row_count);
        float y = _padding.top;
        size_t row_stretched_index = 0;
        for (size_t r = 0; r < row_count; ++r)
        {
            const float stretch = get_row_stretch(r);
            float size = stretch > 0.0f && row_stretch_sum > 0.0f
                ? row_stretched_sizes[row_stretched_index++]
                : row_preferred[r];
            size = std::max(size, row_minimum[r]);
            row_offset[r] = y;
            row_size[r] = size;
            y += size + _row_gap;
        }

        std::vector<float> column_size(column_count);
        std::vector<float> column_offset(column_count);
        float x = _padding.left;
        size_t column_stretched_index = 0;
        for (size_t c = 0; c < column_count; ++c)
        {
            const float stretch = get_column_stretch(c);
            float size = stretch > 0.0f && column_stretch_sum > 0.0f
                ? column_stretched_sizes[column_stretched_index++]
                : column_preferred[c];
            size = std::max(size, column_minimum[c]);
            column_offset[c] = x;
            column_size[c] = size;
            x += size + _column_gap;
        }

        for (size_t r = 0; r < row_count; ++r)
        {
            for (size_t c = 0; c < column_count; ++c)
            {
                Face* const cell = _grid[r][c];
                if (!cell)
                    continue;

                // A spanning cell is only actually arranged once, at its own
                // origin, where its combined rectangle is the union of
                // every row/column it covers; every other cell it covers
                // just defers to that (see this class's own doc comment).
                size_t origin_row = r;
                size_t origin_column = c;
                size_t row_span = 1;
                size_t column_span = 1;
                const auto span_it = _spans.find(cell);
                if (span_it != _spans.end())
                {
                    origin_row = span_it->second.row;
                    origin_column = span_it->second.column;
                    row_span = span_it->second.row_span;
                    column_span = span_it->second.column_span;
                    if (origin_row != r || origin_column != c)
                        continue;
                }

                float column_position = column_offset[origin_column];
                float column_extent = _column_gap * static_cast<float>(column_span - 1);
                for (size_t i = 0; i < column_span; ++i)
                    column_extent += column_size[origin_column + i];

                float row_position = row_offset[origin_row];
                float row_extent = _row_gap * static_cast<float>(row_span - 1);
                for (size_t i = 0; i < row_span; ++i)
                    row_extent += row_size[origin_row + i];

                const ml7::Vector2f preferred_size = cell->get_effective_preferred_size();
                const ml7::Vector2f minimum_size = cell->get_effective_minimum_size();
                const Anchor anchor = cell->get_anchor();

                ml7::Vector2f position = cell->get_position();
                ml7::Vector2f size = cell->get_size();

                // Fits the cell's own child within it, per its own anchor,
                // independently per axis; see BoxLayout's own doc comment
                // for the exact rules (identical here).
                _fit_in_cell(anchor, Anchor::Left, Anchor::Right, column_position, column_extent, preferred_size.x, minimum_size.x, position.x, size.x);
                _fit_in_cell(anchor, Anchor::Top, Anchor::Bottom, row_position, row_extent, preferred_size.y, minimum_size.y, position.y, size.y);

                if (position != cell->get_position())
                    cell->set_position(position);
                if (size != cell->get_size())
                    cell->set_size(size);
            }
        }
    }

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
    ml7::Vector2f GridLayout::calculate_preferred_size(const Container& container) const
    {
        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        const auto [row_preferred, row_minimum, column_preferred, column_minimum] = _measure_tracks();

        float height = _padding.top + _padding.bottom + (row_count > 0 ? _row_gap * static_cast<float>(row_count - 1) : 0.0f);
        for (const float h : row_preferred)
            height += h;

        float width = _padding.left + _padding.right + (column_count > 0 ? _column_gap * static_cast<float>(column_count - 1) : 0.0f);
        for (const float w : column_preferred)
            width += w;

        return {width, height};
    }

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
    ml7::Vector2f GridLayout::calculate_minimum_size(const Container& container) const
    {
        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        const auto [row_preferred, row_minimum, column_preferred, column_minimum] = _measure_tracks();

        float height = _padding.top + _padding.bottom + (row_count > 0 ? _row_gap * static_cast<float>(row_count - 1) : 0.0f);
        for (size_t r = 0; r < row_count; ++r)
            height += get_row_stretch(r) > 0.0f ? row_minimum[r] : row_preferred[r];

        float width = _padding.left + _padding.right + (column_count > 0 ? _column_gap * static_cast<float>(column_count - 1) : 0.0f);
        for (size_t c = 0; c < column_count; ++c)
            width += get_column_stretch(c) > 0.0f ? column_minimum[c] : column_preferred[c];

        return {width, height};
    }



    // #############################################################################
    // Helpers
    // #############################################################################

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
    GridLayout::TrackExtents GridLayout::_measure_tracks() const
    {
        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        TrackExtents extents;
        extents.row_preferred.assign(row_count, 0.0f);
        extents.row_minimum.assign(row_count, 0.0f);
        extents.column_preferred.assign(column_count, 0.0f);
        extents.column_minimum.assign(column_count, 0.0f);

        // Every cell that doesn't itself span a given axis contributes to
        // that axis's own row/column max, same as if there were no spans.
        for (size_t r = 0; r < row_count; ++r)
        {
            for (size_t c = 0; c < column_count; ++c)
            {
                Face* const cell = _grid[r][c];
                if (!cell)
                    continue;

                const auto span_it = _spans.find(cell);
                const size_t row_span = span_it != _spans.end() ? span_it->second.row_span : 1;
                const size_t column_span = span_it != _spans.end() ? span_it->second.column_span : 1;

                const ml7::Vector2f preferred = cell->get_effective_preferred_size();
                const ml7::Vector2f minimum = cell->get_effective_minimum_size();

                if (row_span == 1)
                {
                    extents.row_preferred[r] = std::max(extents.row_preferred[r], preferred.y);
                    extents.row_minimum[r] = std::max(extents.row_minimum[r], minimum.y);
                }
                if (column_span == 1)
                {
                    extents.column_preferred[c] = std::max(extents.column_preferred[c], preferred.x);
                    extents.column_minimum[c] = std::max(extents.column_minimum[c], minimum.x);
                }
            }
        }

        // Every spanning child, grown into whichever axis/axes it actually
        // spans, shortest span first (per axis) to keep this independent
        // of insertion order.
        std::vector<const SpanEntry*> row_spanning;
        std::vector<const SpanEntry*> column_spanning;
        for (const auto& entry : _spans)
        {
            if (entry.second.row_span > 1)
                row_spanning.push_back(&entry.second);
            if (entry.second.column_span > 1)
                column_spanning.push_back(&entry.second);
        }
        std::stable_sort(row_spanning.begin(), row_spanning.end(),
            [](const SpanEntry* a, const SpanEntry* b) { return a->row_span < b->row_span; });
        std::stable_sort(column_spanning.begin(), column_spanning.end(),
            [](const SpanEntry* a, const SpanEntry* b) { return a->column_span < b->column_span; });

        for (const SpanEntry* span : row_spanning)
        {
            Face* const cell = _grid[span->row][span->column];
            _grow_spanned_tracks(extents.row_preferred, span->row, span->row_span, _row_gap, cell->get_effective_preferred_size().y);
            _grow_spanned_tracks(extents.row_minimum, span->row, span->row_span, _row_gap, cell->get_effective_minimum_size().y);
        }
        for (const SpanEntry* span : column_spanning)
        {
            Face* const cell = _grid[span->row][span->column];
            _grow_spanned_tracks(extents.column_preferred, span->column, span->column_span, _column_gap, cell->get_effective_preferred_size().x);
            _grow_spanned_tracks(extents.column_minimum, span->column, span->column_span, _column_gap, cell->get_effective_minimum_size().x);
        }

        return extents;
    }

    /**
     * Grows `sizes[start, start + span)` evenly until their sum (plus `gap` kept
     * between them) reaches `own_extent`, or leaves them as they are if it already
     * does. Shared by `_measure_tracks()`'s own row and column passes, otherwise
     * identical.
     */
    void GridLayout::_grow_spanned_tracks(std::vector<float>& sizes, size_t start, size_t span, float gap, float own_extent)
    {
        float combined = gap * static_cast<float>(span - 1);
        for (size_t i = 0; i < span; ++i)
            combined += sizes[start + i];

        if (combined >= own_extent)
            return;

        const float added = (own_extent - combined) / static_cast<float>(span);
        for (size_t i = 0; i < span; ++i)
            sizes[start + i] += added;
    }



} // namespace fl7::gui::layout
