#include "Container.h"

#include <algorithm>



namespace fl7::gui {



    // #############################################################################
    // Collection Implementations
    // #############################################################################

    /**
     * While no layout is attached (see `get_layout()`), shifts every child inward
     * by this container's own effective padding's left/top (see
     * `get_effective_padding()`): a child's own `get_position()` at `{0, 0}` then
     * actually renders/is hit-tested at `{padding.left, padding.top}`, same as any
     * other `Collection::get_scroll_offset()` shift. `{0, 0}` (no shift) instead
     * once a layout is attached: see this class's own doc comment.
     */
    ml7::Vector2f Container::get_scroll_offset() const
    {
        if (get_layout())
            return Collection::get_scroll_offset();

        const Padding padding = get_effective_padding();
        return {-padding.left, -padding.top}; // Negated: see Collection::get_scroll_offset()'s own sign convention.
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * With no layout set (children just keep whatever position/size they're
     * explicitly given; see this class's own doc comment), returns the tightest
     * bounding box enclosing every child's own `get_position()` plus its own
     * `get_effective_preferred_size()`, recursively (a child's own effective
     * preferred size may itself come from this same calculation, if it's a
     * `Container` too). With a layout set, defers to its own
     * `layout::Layout::calculate_preferred_size()` instead.
     */
    ml7::Vector2f Container::_calculate_preferred_size() const
    {
        if (const layout::Layout* layout = get_layout())
            return layout->calculate_preferred_size(*this);

        ml7::Vector2f size;
        for (const auto& child : get_children())
        {
            const ml7::Vector2f extent = child->get_position() + child->get_effective_preferred_size();
            size.x = std::max(size.x, extent.x);
            size.y = std::max(size.y, extent.y);
        }
        return size;
    }

    /**
     * With no layout set (children just keep whatever position/size they're
     * explicitly given; see this class's own doc comment), returns the tightest
     * bounding box enclosing every child's own `get_position()` plus its own
     * `get_effective_minimum_size()`, recursively (a child's own effective
     * minimum size may itself come from this same calculation, if it's a
     * `Container` too). With a layout set, defers to its own
     * `layout::Layout::calculate_minimum_size()` instead.
     */
    ml7::Vector2f Container::_calculate_minimum_size() const
    {
        if (const layout::Layout* layout = get_layout())
            return layout->calculate_minimum_size(*this);

        ml7::Vector2f size;
        for (const auto& child : get_children())
        {
            const ml7::Vector2f extent = child->get_position() + child->get_effective_minimum_size();
            size.x = std::max(size.x, extent.x);
            size.y = std::max(size.y, extent.y);
        }
        return size;
    }



} // namespace fl7::gui
