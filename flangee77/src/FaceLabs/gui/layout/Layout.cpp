#include "Layout.h"

#include <algorithm>



namespace fl7::gui::layout {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Fits a child's own preferred size within its cell, along a single axis,
     * never below `minimum`, per anchor's `near_flag` and `far_flag`. Shared by
     * every concrete layout that divides its container into anchor-fitted
     * cells (see `BoxLayout::perform()` or `GridLayout::perform()`).
     */
    void Layout::_fit_in_cell(Anchor anchor, Anchor near_flag, Anchor far_flag, float cell_position, float cell_size, float preferred, float minimum, float& out_position, float& out_size)
    {
        if (cell_size <= preferred)
        {
            // No slack to work with either way: shrinks to the cell, but never
            // below the child's own minimum size, even if that overflows it.
            out_position = cell_position;
            out_size = std::max(cell_size, minimum);
            return;
        }

        const bool near = (anchor & near_flag) != Anchor::None;
        const bool far = (anchor & far_flag) != Anchor::None;

        if (near && far)
        {
            out_position = cell_position;
            out_size = cell_size;
        }
        else if (far)
        {
            out_position = cell_position + (cell_size - preferred);
            out_size = preferred;
        }
        else if (near)
        {
            out_position = cell_position;
            out_size = preferred;
        }
        else
        {
            out_position = cell_position + (cell_size - preferred) * 0.5f;
            out_size = preferred;
        }
    }

    /**
     * Splits `available` among `stretch.size()` stretched contributors
     * (`stretch`/`minimum` parallel, one entry per contributor), proportional
     * to each one's own `stretch`, but never below its own `minimum`: any
     * contributor whose plain ratio-based share would undercut that instead gets
     * exactly its own `minimum`, with the leftover (space and stretch)
     * redistributed among the remaining, not-yet-undercut contributors the same way,
     * repeated until none are left undercut (same idea as CSS flexbox's own
     * "resolve flexible lengths" algorithm); a contributor only ever actually ends
     * up below its own `minimum` when every contributor's own `minimum`, summed,
     * already exceeds `available` to begin with (then there's simply no way to
     * honor every one, regardless). Shared by `BoxLayout::perform()` (one call, its
     * own main axis) and `GridLayout::perform()` (one call per axis, rows and
     * columns independently), both otherwise prone to overcommitting `available` by
     * individually flooring an undercut contributor at its own `minimum` without
     * taking that same amount back from the others.
     */
    std::vector<float> Layout::_distribute_stretched_space(float available, const std::vector<float>& stretch, const std::vector<float>& minimum)
    {
        const size_t n = stretch.size();
        std::vector<float> size(n, 0.0f);
        std::vector<bool> frozen(n, false);

        float remaining = available;
        float stretch_sum = 0.0f;
        for (const float s : stretch)
            stretch_sum += s;

        // Repeatedly freezes every contributor whose plain ratio-based share
        // (against the current remaining/stretch_sum) would undercut its own
        // minimum, at exactly that minimum, taking both back out of the pool
        // for the next pass; stops once a pass freezes none.
        for (bool changed = true; changed && stretch_sum > 0.0f; )
        {
            changed = false;
            float frozen_minimum_total = 0.0f;
            float frozen_stretch_total = 0.0f;

            for (size_t i = 0; i < n; ++i)
            {
                if (frozen[i])
                    continue;
                if (remaining * (stretch[i] / stretch_sum) < minimum[i])
                {
                    size[i] = minimum[i];
                    frozen[i] = true;
                    frozen_minimum_total += minimum[i];
                    frozen_stretch_total += stretch[i];
                    changed = true;
                }
            }

            remaining -= frozen_minimum_total;
            stretch_sum -= frozen_stretch_total;
        }

        if (stretch_sum > 0.0f)
            for (size_t i = 0; i < n; ++i)
                if (!frozen[i])
                    size[i] = remaining * (stretch[i] / stretch_sum);

        return size;
    }



} // namespace fl7::gui::layout
