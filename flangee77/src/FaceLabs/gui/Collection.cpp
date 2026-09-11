#include "Collection.h"

#include <ranges>



namespace fl7::gui {



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Finds the topmost face containing the given absolute (screen) point.
     */
    Face* Collection::_find_hit_face(ml7::Vector2f screen_point, ml7::Vector2f parent_absolute_position)
    {
        if (!is_visible() || !is_enabled())
            return nullptr;

        const ml7::Vector2f absolute_position = parent_absolute_position + get_position();
        const ml7::Vector2f local_point = screen_point - absolute_position;

        // A child clipped out of view (see clips_children()) isn't hit-testable
        // either: this always tests the same axis-aligned bounds used for the
        // actual visual clipping, regardless of this collection's own
        // (possibly non-rectangular) _contains_point() hit shape.
        const bool children_reachable = !_clips_children
            || (local_point.x >= 0.0f && local_point.y >= 0.0f && local_point.x < get_size().x && local_point.y < get_size().y);

        if (children_reachable)
        {
            // Children first, most-recently-added (topmost) first.
            for (const auto& it : std::views::reverse(_children))
            {
                if (Face* hit = it->_find_hit_face(screen_point, absolute_position))
                    return hit;
            }
        }

        return _contains_point(local_point) ? this : nullptr;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Adds the specified face as a child of this Collection.
     */
    void Collection::_add_child(std::unique_ptr<Face> child)
    {
        child->_parent = this;
        _children.push_back(std::move(child));
    }



} // namespace fl7::gui
