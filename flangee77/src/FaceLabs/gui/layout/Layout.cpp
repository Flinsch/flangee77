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



} // namespace fl7::gui::layout
