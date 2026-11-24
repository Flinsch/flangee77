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

        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        if (row_count == 0 || column_count == 0)
            return;

        // Every row's own tallest preferred/minimum height, every column's
        // own widest preferred/minimum width, across their own cells.
        std::vector<float> row_preferred(row_count, 0.0f);
        std::vector<float> row_minimum(row_count, 0.0f);
        std::vector<float> column_preferred(column_count, 0.0f);
        std::vector<float> column_minimum(column_count, 0.0f);
        for (size_t r = 0; r < row_count; ++r)
        {
            for (size_t c = 0; c < column_count; ++c)
            {
                Face* const cell = _grid[r][c];
                if (!cell)
                    continue;

                const ml7::Vector2f preferred = cell->get_effective_preferred_size();
                const ml7::Vector2f minimum = cell->get_effective_minimum_size();
                row_preferred[r] = std::max(row_preferred[r], preferred.y);
                row_minimum[r] = std::max(row_minimum[r], minimum.y);
                column_preferred[c] = std::max(column_preferred[c], preferred.x);
                column_minimum[c] = std::max(column_minimum[c], minimum.x);
            }
        }

        const ml7::Vector2f container_size = container.get_size();
        const float total_row_gap = _row_gap * static_cast<float>(row_count - 1);
        const float total_column_gap = _column_gap * static_cast<float>(column_count - 1);

        // Distributes the available height/width across stretched rows/columns,
        // same idea as BoxLayout::perform(), just independently per axis.
        float row_fixed_total = 0.0f;
        float row_stretch_sum = 0.0f;
        for (size_t r = 0; r < row_count; ++r)
        {
            const float stretch = get_row_stretch(r);
            if (stretch > 0.0f)
                row_stretch_sum += stretch;
            else
                row_fixed_total += row_preferred[r];
        }

        float column_fixed_total = 0.0f;
        float column_stretch_sum = 0.0f;
        for (size_t c = 0; c < column_count; ++c)
        {
            const float stretch = get_column_stretch(c);
            if (stretch > 0.0f)
                column_stretch_sum += stretch;
            else
                column_fixed_total += column_preferred[c];
        }

        const float row_available = std::max(0.0f, container_size.y - _padding.top - _padding.bottom - total_row_gap - row_fixed_total);
        const float column_available = std::max(0.0f, container_size.x - _padding.left - _padding.right - total_column_gap - column_fixed_total);

        std::vector<float> row_size(row_count);
        std::vector<float> row_offset(row_count);
        float y = _padding.top;
        for (size_t r = 0; r < row_count; ++r)
        {
            const float stretch = get_row_stretch(r);
            float size = stretch > 0.0f && row_stretch_sum > 0.0f
                ? row_available * (stretch / row_stretch_sum)
                : row_preferred[r];
            size = std::max(size, row_minimum[r]);
            row_offset[r] = y;
            row_size[r] = size;
            y += size + _row_gap;
        }

        std::vector<float> column_size(column_count);
        std::vector<float> column_offset(column_count);
        float x = _padding.left;
        for (size_t c = 0; c < column_count; ++c)
        {
            const float stretch = get_column_stretch(c);
            float size = stretch > 0.0f && column_stretch_sum > 0.0f
                ? column_available * (stretch / column_stretch_sum)
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

                const ml7::Vector2f preferred_size = cell->get_effective_preferred_size();
                const ml7::Vector2f minimum_size = cell->get_effective_minimum_size();
                const Anchor anchor = cell->get_anchor();

                ml7::Vector2f position = cell->get_position();
                ml7::Vector2f size = cell->get_size();

                // Fits the cell's own child within it, per its own anchor,
                // independently per axis; see BoxLayout's own doc comment
                // for the exact rules (identical here).
                _fit_in_cell(anchor, Anchor::Left, Anchor::Right, column_offset[c], column_size[c], preferred_size.x, minimum_size.x, position.x, size.x);
                _fit_in_cell(anchor, Anchor::Top, Anchor::Bottom, row_offset[r], row_size[r], preferred_size.y, minimum_size.y, position.y, size.y);

                if (position != cell->get_position())
                    cell->set_position(position);
                if (size != cell->get_size())
                    cell->set_size(size);
            }
        }
    }

    /**
     * Returns the sum of every row's own tallest effective preferred height
     * (plus the fixed gaps between rows, plus this layout's own padding on
     * that axis) by the sum of every column's own widest effective preferred
     * width (plus the fixed gaps between columns, plus padding there): the
     * size this grid would occupy if every row/column actually got exactly
     * its own preferred extent, regardless of stretch (which only ever
     * redistributes space beyond or below that, see `perform()`).
     */
    ml7::Vector2f GridLayout::calculate_preferred_size(const Container& container) const
    {
        return _calculate_size(&Face::get_effective_preferred_size);
    }

    /**
     * Returns the sum of every row's own tallest effective minimum height
     * (plus the fixed gaps between rows, plus this layout's own padding on
     * that axis) by the sum of every column's own widest effective minimum
     * width (plus the fixed gaps between columns, plus padding there): the
     * size this grid would occupy if every row/column actually got exactly
     * its own minimum extent, regardless of stretch (which only ever
     * redistributes space beyond or below that, see `perform()`).
     */
    ml7::Vector2f GridLayout::calculate_minimum_size(const Container& container) const
    {
        return _calculate_size(&Face::get_effective_minimum_size);
    }

    /**
     * Shared by `calculate_preferred_size()` or `calculate_minimum_size()`:
     * sums the tallest/widest `size_of(cell)` across every row/column of this
     * layout's own grid (plus gaps/padding on each axis), for whichever
     * per-child size getter the caller passes in.
     */
    ml7::Vector2f GridLayout::_calculate_size(ml7::Vector2f (Face::*size_of)() const) const
    {
        const size_t row_count = _grid.size();
        const size_t column_count = _column_count;

        std::vector<float> row_size(row_count, 0.0f);
        std::vector<float> column_size(column_count, 0.0f);

        for (size_t r = 0; r < row_count; ++r)
        {
            for (size_t c = 0; c < column_count; ++c)
            {
                Face* const cell = _grid[r][c];
                if (!cell)
                    continue;

                const ml7::Vector2f size = (cell->*size_of)();
                row_size[r] = std::max(row_size[r], size.y);
                column_size[c] = std::max(column_size[c], size.x);
            }
        }

        float height = _padding.top + _padding.bottom + (row_count > 0 ? _row_gap * static_cast<float>(row_count - 1) : 0.0f);
        for (const float h : row_size)
            height += h;

        float width = _padding.left + _padding.right + (column_count > 0 ? _column_gap * static_cast<float>(column_count - 1) : 0.0f);
        for (const float w : column_size)
            width += w;

        return {width, height};
    }



} // namespace fl7::gui::layout
