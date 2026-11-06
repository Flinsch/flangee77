#include "BoxLayout.h"

#include "../Container.h"
#include "../Face.h"

#include <algorithm>



namespace fl7::gui::layout {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the given child's stretch factor; see `set_stretch()`. Default: 0
     * (not stretched: takes exactly its own `get_preferred_size()`).
     */
    float BoxLayout::get_stretch(const Face& child) const
    {
        const auto it = _stretches.find(&child);
        return it != _stretches.end() ? it->second : 0.0f;
    }

    /**
     * Sets the given child's stretch factor: 0 (the default) or less means it's not
     * stretched at all, taking exactly its own `get_preferred_size()` along this
     * layout's own axis; a positive value means it instead takes that share of
     * whatever space is left over once every non-stretched child has taken its own
     * preferred size, relative to the other stretched children's own factors (e.g.,
     * two children both set to 1 split the leftover space evenly; one set to 2 and
     * another to 1 split it two-to-one).
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
        const auto& children = container.get_children();
        const size_t n = children.size();
        if (n == 0)
            return;

        const unsigned axis = _orientation == Orientation::Horizontal ? 0 : 1;
        const unsigned cross_axis = 1 - axis;

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
                fixed_total += child_ptr->get_preferred_size()[axis];
        }

        const float available = std::max(0.0f, container_size[axis] - total_gap - fixed_total);

        float offset = 0.0f;
        for (const auto& child_ptr : children)
        {
            Face& child = *child_ptr;

            const ml7::Vector2f preferred_size = child.get_preferred_size();
            const ml7::Vector2f minimum_size = child.get_minimum_size();

            const float stretch = get_stretch(child);
            float main_size = stretch > 0.0f && stretch_sum > 0.0f
                ? available * (stretch / stretch_sum)
                : preferred_size[axis];
            main_size = std::max(main_size, minimum_size[axis]);

            ml7::Vector2f position = child.get_position();
            ml7::Vector2f size = child.get_size();
            position[axis] = offset;
            size[axis] = main_size;

            // Cross axis: sized/aligned per the child's own anchor (see
            // BoxLayout's own doc comment); falls back to filling if its
            // preferred size doesn't actually fit regardless.
            const float cross_available = container_size[cross_axis];
            const float cross_preferred = preferred_size[cross_axis];
            if (cross_preferred < cross_available)
            {
                const Anchor anchor = child.get_anchor();
                const Anchor near_flag = cross_axis == 0 ? Anchor::Left : Anchor::Top;
                const Anchor far_flag = cross_axis == 0 ? Anchor::Right : Anchor::Bottom;
                const bool near = (anchor & near_flag) != Anchor::None;
                const bool far = (anchor & far_flag) != Anchor::None;

                size[cross_axis] = cross_preferred;
                if (near && far)
                {
                    position[cross_axis] = 0.0f;
                    size[cross_axis] = cross_available; // still fills.
                }
                else if (far)
                {
                    position[cross_axis] = cross_available - cross_preferred;
                }
                else if (near)
                {
                    position[cross_axis] = 0.0f;
                }
                else
                {
                    position[cross_axis] = (cross_available - cross_preferred) * 0.5f;
                }
            }
            else
            {
                size[cross_axis] = cross_available;
                position[cross_axis] = 0.0f;
            }

            if (position != child.get_position())
                child.set_position(position);
            if (size != child.get_size())
                child.set_size(size);

            offset += main_size + _gap;
        }
    }



} // namespace fl7::gui::layout
