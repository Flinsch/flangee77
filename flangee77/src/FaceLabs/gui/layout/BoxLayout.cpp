#include "BoxLayout.h"

#include "../Container.h"
#include "../Face.h"

#include <algorithm>
#include <unordered_set>



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
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the
     * sequencing/stretching/cross-axis-alignment rules described on this class.
     */
    void BoxLayout::perform(Container& container)
    {
        // Prunes _order/_stretches of any child no longer actually present.
        std::unordered_set<const Face*> current;
        for (const auto& child : container.get_children())
            current.insert(child.get());
        std::erase_if(_order, [&current](const Face* child) { return !current.contains(child); });
        for (auto it = _stretches.begin(); it != _stretches.end(); )
            it = current.contains(it->first) ? std::next(it) : _stretches.erase(it);

        const size_t n = _order.size();

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
        for (const auto& child_ptr : _order)
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
        for (const auto& child_ptr : _order)
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
            _fit_in_cell(anchor, main_near_flag, main_far_flag, offset, cell_main_size, preferred_size[axis], minimum_size[axis], position[axis], size[axis]);
            _fit_in_cell(anchor, cross_near_flag, cross_far_flag, padding_cross_near, cross_available, preferred_size[cross_axis], minimum_size[cross_axis], position[cross_axis], size[cross_axis]);

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

    /**
     * Returns the sum of every child's own effective preferred size along this
     * layout's own main axis (plus the fixed gaps between them, plus this layout's
     * own padding on that axis), and the largest of their own effective preferred
     * sizes along the cross axis (plus padding there): the size this sequence of
     * children would occupy if every one of them actually got exactly its own
     * preferred size, regardless of stretch (which only ever redistributes space
     * beyond or below that, see `perform()`).
     */
    ml7::Vector2f BoxLayout::calculate_preferred_size(const Container& container) const
    {
        const size_t n = _order.size();

        const unsigned axis = _orientation == Orientation::Horizontal ? 0 : 1;
        const unsigned cross_axis = 1 - axis;

        const float padding_main = axis == 0 ? _padding.left + _padding.right : _padding.top + _padding.bottom;
        const float padding_cross = cross_axis == 0 ? _padding.left + _padding.right : _padding.top + _padding.bottom;

        float main = padding_main + (n > 0 ? _gap * static_cast<float>(n - 1) : 0.0f);
        float cross = 0.0f;
        for (const auto& child_ptr : _order)
        {
            const ml7::Vector2f preferred = child_ptr->get_effective_preferred_size();
            main += preferred[axis];
            cross = std::max(cross, preferred[cross_axis]);
        }
        cross += padding_cross;

        ml7::Vector2f result;
        result[axis] = main;
        result[cross_axis] = cross;
        return result;
    }

    /**
     * Returns the smallest this sequence could ever actually be laid out at
     * without overflowing, along this layout's own main axis (plus the fixed
     * gaps between children, plus this layout's own padding on that axis):
     * every non-stretched (stretch <= 0) child's own effective preferred size
     * (that's what `perform()` always gives it, never less), plus every
     * stretched child's own effective minimum size instead (the only ones
     * `perform()` can actually shrink that far). Along the cross axis: the
     * largest of every child's own effective minimum size there (plus padding),
     * same as before, since the cross axis has no stretch distinction to begin
     * with.
     */
    ml7::Vector2f BoxLayout::calculate_minimum_size(const Container& container) const
    {
        const size_t n = _order.size();

        const unsigned axis = _orientation == Orientation::Horizontal ? 0 : 1;
        const unsigned cross_axis = 1 - axis;

        const float padding_main = axis == 0 ? _padding.left + _padding.right : _padding.top + _padding.bottom;
        const float padding_cross = cross_axis == 0 ? _padding.left + _padding.right : _padding.top + _padding.bottom;

        float main = padding_main + (n > 0 ? _gap * static_cast<float>(n - 1) : 0.0f);
        float cross = 0.0f;
        for (const auto& child_ptr : _order)
        {
            const Face& child = *child_ptr;
            const ml7::Vector2f minimum = child.get_effective_minimum_size();
            main += get_stretch(child) > 0.0f ? minimum[axis] : child.get_effective_preferred_size()[axis];
            cross = std::max(cross, minimum[cross_axis]);
        }
        cross += padding_cross;

        ml7::Vector2f result;
        result[axis] = main;
        result[cross_axis] = cross;
        return result;
    }



} // namespace fl7::gui::layout
