#include "Face.h"

#include "./HasCheckedState.h"
#include "./Shell.h"
#include "./Theme.h"



namespace fl7::gui {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the shell this face (transitively) belongs to, or `nullptr` if it
     * isn't (yet) part of one. Only ever actually set (by Shell::add_face()) on a
     * top-level face, so this walks up the parent chain to find it. Not cached
     * (fine at GUI scale.
     */
    Shell* Face::get_shell() const
    {
        for (const Face* face = this; face; face = face->_parent)
            if (face->_shell)
                return face->_shell;

        return nullptr;
    }

    /**
     * Sets this face's size.
     */
    void Face::set_size(ml7::Vector2f size)
    {
        if (size == _size)
            return;

        const ml7::Vector2f old_size = _size;
        _size = size;
        _on_size_changed(old_size, size);
    }

    /**
     * Returns this face's absolute (screen) position, computed by walking up the
     * parent chain. Not cached (fine at GUI scale, matches the rest of the
     * traversal/hit-testing code).
     */
    ml7::Vector2f Face::get_absolute_position() const
    {
        ml7::Vector2f position = _position;
        for (const Face* parent = _parent; parent; parent = parent->_parent)
            position += parent->_position;
        return position;
    }

    /**
     * Returns this face's effective style: its own override if set, else the
     * nearest ancestor's explicit override (not that ancestor's own *effective*
     * style, which may itself just be a theme resolution, that's specific to the
     * role of the ancestor, not to the role of this face), else this face's own
     * role resolved against the owning shell's theme (using its own checked/
     * hovered/pressed/focused state, see Theme::State, unless
     * _get_interaction_state_proxy() redirects that part to another face), else a
     * default-constructed style as a last resort (no override anywhere up the
     * chain, no theme, or a face type with no themed role).
     */
    Style Face::get_effective_style() const
    {
        Style style = _resolve_effective_style();
        _adjust_effective_text_style(style.text_style);
        return style;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the nearest ancestor's explicit style override, walking up the parent
     * chain, or `nullptr` if none of them have one set.
     */
    const Style* Face::_find_inherited_style_override() const
    {
        for (const Face* parent = _parent; parent; parent = parent->_parent)
            if (parent->_style_override)
                return &*parent->_style_override;

        return nullptr;
    }

    /**
     * Does the actual work of get_effective_style(), minus the final
     * _adjust_effective_text_style() pass.
     */
    Style Face::_resolve_effective_style() const
    {
        if (_style_override)
            return *_style_override;

        if (const Style* inherited = _find_inherited_style_override())
            return *inherited;

        const cl7::u8string_view theme_key = _get_theme_key();
        if (!theme_key.empty())
        {
            if (Shell* shell = get_shell())
            {
                if (const Theme* theme = shell->get_theme())
                {
                    const Face* const proxy = _get_interaction_state_proxy();
                    const Face* const state_face = proxy ? proxy : this;

                    const auto* checkable = dynamic_cast<const HasCheckedState*>(state_face);
                    Face* const pressed_face = shell->get_pressed_face();
                    const bool is_hovered = shell->get_hovered_face() == state_face;
                    const bool is_pressed = pressed_face == state_face;
                    const bool is_focused = shell->get_focused_face() == state_face;

                    return theme->resolve(theme_key, {
                        .checked = checkable && checkable->is_checked(),
                        // Mouse capture (see Shell::_pressed_face) deliberately stays
                        // "pressed" for the whole gesture even once the cursor leaves
                        // the face (e.g., Frame's drag relies on that), but the
                        // visual pressed state should only show while the cursor is
                        // actually still over the face, like every other GUI's button
                        // does (drag off while held to "cancel" the visual, drag back
                        // to "reinstate" it, any number of times). Symmetrically,
                        // "hovered" shouldn't visually apply to some other face while
                        // a press is captured elsewhere either (e.g., dragging off a
                        // pressed button onto a second one shouldn't light up that
                        // second one), so hover only counts while nothing else is
                        // captured, or this is the face that is.
                        .hovered = is_hovered && (!pressed_face || is_pressed),
                        .pressed = is_hovered && is_pressed,
                        .focused = is_focused,
                    });
                }
            }
        }

        return {};
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
