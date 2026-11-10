#include "BoxLayout.h"

#include "../Container.h"
#include "../Face.h"

#include <algorithm>



namespace fl7::gui::layout {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the given child's stretch factor; see `set_stretch()`. Default: 1
     * (every child, until told otherwise, participates evenly in distributing the
     * main-axis space).
     */
    float BoxLayout::get_stretch(const Face& child) const
    {
        const auto it = _stretches.find(&child);
        return it != _stretches.end() ? it->second : 1.0f;
    }

    /**
     * Sets the given child's stretch factor: 0 or less means its cell is exactly
     * its own `get_effective_preferred_size()` along this layout's own axis, not
     * participating in the distribution at all; a positive value (1, the default
     * for every child until told otherwise) means its cell instead takes that
     * share of whatever space is left over once every non-stretched (stretch <= 0)
     * child's cell has taken its own preferred size, relative to the other
     * stretched children's own factors (e.g., two children both left at the
     * default 1 split the leftover space evenly; one set to 2 and another left at
     * 1 split it two-to-one). Either way, purely about the cell's own size,
     * independent of whether the child actually fills it (see this class's own
     * doc comment: that's its anchor's call, not this one's).
     */
    void BoxLayout::set_stretch(const Face& child, float stretch)
    {
        _stretches[&child] = stretch;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Fits a child's own preferred size within its cell, along a single axis,
     * per anchor's near_flag/far_flag; see this class's own doc comment and
     * this method's own declaration (BoxLayout.h) for the exact rules.
     */
    void BoxLayout::_fit_in_cell(Anchor anchor, Anchor near_flag, Anchor far_flag, float cell_position, float cell_size, float preferred, float& out_position, float& out_size)
    {
        if (cell_size <= preferred)
        {
            // No slack to work with either way: just fills the cell.
            out_position = cell_position;
            out_size = cell_size;
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



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the
     * sequencing/stretching/cross-axis-alignment rules described on this class.
     */
    void BoxLayout::perform(Container& container)
    {
        const auto& children = container.get_children();
        const size_t n = children.size();

        if (n == 0)
            return;

        const unsigned axis = _orientation == Orientation::Horizontal ? 0 : 1;
        const unsigned cross_axis = 1 - axis;

        const float padding_main_near = axis == 0 ? _padding.left : _padding.top;
        const float padding_main_far = axis == 0 ? _padding.right : _padding.bottom;
        const float padding_cross_near = cross_axis == 0 ? _padding.left : _padding.top;
        const float padding_cross_far = cross_axis == 0 ? _padding.right : _padding.bottom;

        const ml7::Vector2f container_size = container.get_size();
        const float total_gap = _gap * static_cast<float>(n - 1);

        float fixed_total = 0.0f;
        float stretch_sum = 0.0f;
        for (const auto& child_ptr : children)
        {
            const float stretch = get_stretch(*child_ptr);
            if (stretch > 0.0f)
                stretch_sum += stretch;
            else
                fixed_total += child_ptr->get_effective_preferred_size()[axis];
        }

        const float available = std::max(0.0f, container_size[axis] - padding_main_near - padding_main_far - total_gap - fixed_total);
        const float cross_available = std::max(0.0f, container_size[cross_axis] - padding_cross_near - padding_cross_far);

        float offset = padding_main_near;
        for (const auto& child_ptr : children)
        {
            Face& child = *child_ptr;

            const ml7::Vector2f preferred_size = child.get_effective_preferred_size();
            const ml7::Vector2f minimum_size = child.get_effective_minimum_size();

            const float stretch = get_stretch(child);
            float cell_main_size = stretch > 0.0f && stretch_sum > 0.0f
                ? available * (stretch / stretch_sum)
                : preferred_size[axis];
            cell_main_size = std::max(cell_main_size, minimum_size[axis]);

            const Anchor anchor = child.get_anchor();
            const Anchor main_near_flag = axis == 0 ? Anchor::Left : Anchor::Top;
            const Anchor main_far_flag = axis == 0 ? Anchor::Right : Anchor::Bottom;
            const Anchor cross_near_flag = cross_axis == 0 ? Anchor::Left : Anchor::Top;
            const Anchor cross_far_flag = cross_axis == 0 ? Anchor::Right : Anchor::Bottom;

            ml7::Vector2f position = child.get_position();
            ml7::Vector2f size = child.get_size();

            // Fits the child's own preferred size within its cell, per its own
            // anchor, independently per axis; see this class's own doc comment.
            _fit_in_cell(anchor, main_near_flag, main_far_flag, offset, cell_main_size, preferred_size[axis], position[axis], size[axis]);
            _fit_in_cell(anchor, cross_near_flag, cross_far_flag, padding_cross_near, cross_available, preferred_size[cross_axis], position[cross_axis], size[cross_axis]);

            if (position != child.get_position())
                child.set_position(position);
            if (size != child.get_size())
                child.set_size(size);

            // Advances by the cell's own size, not whatever the child actually
            // ended up at: a child that doesn't fill its own (larger) cell still
            // occupies that whole cell as far as its siblings' own offsets go.
            offset += cell_main_size + _gap;
        }
    }



} // namespace fl7::gui::layout
