#include "Collection.h"



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

        // Children first, most-recently-added (topmost) first.
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            if (Face* hit = (*it)->_find_hit_face(screen_point, absolute_position))
                return hit;
        }

        const ml7::Vector2f local_point = screen_point - absolute_position;
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
