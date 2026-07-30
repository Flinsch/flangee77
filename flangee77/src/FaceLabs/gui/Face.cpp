#include "Face.h"

#include "./Shell.h"



namespace fl7::gui {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns this face's effective style: its own override if set, else its
     * parent's effective style, else owning shell's default style, else a
     * default-constructed style as a last resort (a face not yet attached to a
     * shell).
     */
    const Style& Face::get_effective_style() const
    {
        if (_style_override)
            return *_style_override;

        if (_parent)
            return _parent->get_effective_style();

        if (_shell)
            return _shell->get_default_style();

        static constexpr Style default_style{};
        return default_style;
    }



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns whether the specified point, in this face's own local coordinate
     * space (i.e., relative to this face's own absolute position), lies within it.
     * Default: an axis-aligned rect test against `get_size`. Override for a
     * non-rectangular hit shape.
     */
    bool Face::_contains_point(ml7::Vector2f local_point) const
    {
        return local_point.x >= 0.0f && local_point.x < _size.x
            && local_point.y >= 0.0f && local_point.y < _size.y;
    }



    // #############################################################################
    // Backend Interface (Shell / Collection)
    // #############################################################################

    /**
     * Finds the topmost face containing the given absolute (screen) point.
     * Base case: a plain Face has no children, so this just tests itself.
     */
    Face* Face::_find_hit_face(ml7::Vector2f screen_point, ml7::Vector2f parent_absolute_position)
    {
        if (!_visible || !_enabled)
            return nullptr;

        const ml7::Vector2f absolute_position = parent_absolute_position + _position;
        const ml7::Vector2f local_point = screen_point - absolute_position;

        return _contains_point(local_point) ? this : nullptr;
    }



} // namespace fl7::gui
