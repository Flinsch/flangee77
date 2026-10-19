#include "Collection.h"

#include "./Shell.h"

#include <algorithm>
#include <ranges>



namespace fl7::gui {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Removes (and destroys, along with its own descendants, if any) the specified
     * child of this collection, which must actually currently be one. Removal is
     * immediate/synchronous, not deferred to some later, safe point: the caller is
     * responsible for never removing a face from within its own (or one of its
     * descendants') callback (_on_click() etc.), which would destroy it mid-call,
     * while one of its own member functions is still running. Clears whichever of
     * the owning Shell's hovered/pressed/focused face (if any) falls within the
     * removed subtree first, so it's never left dangling (see
     * Shell::_handle_face_removed()).
     */
    void Collection::remove_child(Face& child)
    {
        const auto it = std::ranges::find_if(_children, [&child](const std::unique_ptr<Face>& c) {
            return c.get() == &child;
        });
        assert(it != _children.end());

        if (Shell* shell = child.get_shell())
            Shell::Attorney::handle_face_removed(*shell, &child);

        _children.erase(it);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Finds the topmost face containing the given absolute (screen) point: children
     * first (most-recently-added/topmost first), falling back to this collection
     * itself if none of them are hit. Children are tested at the scroll offset,
     * away from where they would otherwise be, same as they they are drawn:
     * scrolling moves where a child effectively is, for every purpose, not just
     * visually.
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
            const ml7::Vector2f children_absolute_position = absolute_position - get_scroll_offset();

            // Children first, most-recently-added (topmost) first.
            for (const auto& it : std::views::reverse(_children))
            {
                if (Face* hit = it->_find_hit_face(screen_point, children_absolute_position))
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
