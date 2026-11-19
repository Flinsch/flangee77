#include "Face.h"

#include "./Collection.h"
#include "./HasCheckedState.h"
#include "./Shell.h"
#include "./Theme.h"



namespace fl7::gui {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the shell this face (transitively) belongs to, or `nullptr` if it
     * isn't (yet) part of one. Only ever actually set (by `Shell::add_face()`) on a
     * top-level face, so this walks up the parent chain to find it. Not cached
     * (fine at GUI scale).
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
     * Returns this face's effective preferred size: how large a layout (see
     * `layout::Layout`) should try to make it, absent any overriding constraint.
     * Falls back, per component, from `get_preferred_size()` to this face's own
     * content-based size (see `_calculate_preferred_size()`) plus
     * `get_effective_padding()` when unset.
     */
    ml7::Vector2f Face::get_effective_preferred_size() const
    {
        const Padding padding = get_effective_padding();
        const ml7::Vector2f calculated = _calculate_preferred_size() + ml7::Vector2f{padding.left + padding.right, padding.top + padding.bottom};
        ml7::Vector2f result = _preferred_size;
        if (result.x <= 0.0f) result.x = calculated.x;
        if (result.y <= 0.0f) result.y = calculated.y;
        return result;
    }

    /**
     * Returns this face's effective minimum size: how small a layout (see
     * `layout::Layout`) is allowed to shrink it while distributing space among
     * several children. Falls back, per component, from `get_minimum_size()` to
     * this face's own content-based size (see `_calculate_minimum_size()`) plus
     * `get_effective_padding()` when unset.
     */
    ml7::Vector2f Face::get_effective_minimum_size() const
    {
        const Padding padding = get_effective_padding();
        const ml7::Vector2f calculated = _calculate_minimum_size() + ml7::Vector2f{padding.left + padding.right, padding.top + padding.bottom};
        ml7::Vector2f result = _minimum_size;
        if (result.x <= 0.0f) result.x = calculated.x;
        if (result.y <= 0.0f) result.y = calculated.y;
        return result;
    }

    /**
     * Returns this face's absolute (screen) position, computed by walking up the
     * parent chain and netting out each ancestor collection's own scroll offset
     * along the way: without that, this would return this face's unscrolled,
     * logical position instead of wherever it's actually drawn/hit-tested right
     * now. Not cached (fine at GUI scale).
     */
    ml7::Vector2f Face::get_absolute_position() const
    {
        ml7::Vector2f position = _position;
        for (const Face* parent = _parent; parent; parent = parent->_parent)
        {
            position += parent->_position;
            if (const auto* collection = dynamic_cast<const Collection*>(parent))
                position -= collection->get_scroll_offset();
        }
        return position;
    }

    /**
     * Returns this face's effective style, resolved in order: this face's own
     * override, if set; else its own role resolved against the owning shell's
     * theme (using checked/hovered/pressed/focused state, see `Theme::State`,
     * possibly redirected via `_get_interaction_state_proxy()` or
     * `_requires_own_bounds_for_hover()`); else, only for a face with no themed
     * role of its own (see `_get_theme_key()`), the nearest ancestor's own
     * explicit override; else a default-constructed style as a last resort. A
     * themed ancestor's override deliberately does not reach past a descendant
     * with its own distinct role (e.g., a button inside an overridden panel
     * still looks like a button), only past anonymous, role-less content.
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
     * Does the actual work of `get_effective_style()`, minus the final
     * `_adjust_effective_text_style()` pass.
     */
    Style Face::_resolve_effective_style() const
    {
        if (_style_override)
            return *_style_override;

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
                    bool is_hovered = shell->get_hovered_face() == state_face;
                    if (is_hovered && _requires_own_bounds_for_hover())
                        is_hovered = _contains_point(shell->get_mouse_position() - get_absolute_position());
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

        if (const Style* inherited = _find_inherited_style_override())
            return *inherited;

        return {};
    }



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns whether the specified point, in this face's own local coordinate
     * space (i.e., relative to this face's own absolute position), lies within it.
     * Default: an axis-aligned rect test against the size. Override for a
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
     * Finds the topmost face containing the given absolute (screen) point, or
     * none. Base case (a plain face has no children): tests only itself. A
     * collection overrides this to check its children first
     * (topmost/most-recently-added first), falling back to this self-test only
     * if none of them are hit.
     */
    Face* Face::_find_hit_face(ml7::Vector2f screen_point, ml7::Vector2f parent_absolute_position)
    {
        if (!_visible || !_enabled)
            return nullptr;

        const ml7::Vector2f absolute_position = parent_absolute_position + _position;
        const ml7::Vector2f local_point = screen_point - absolute_position;

        return _contains_point(local_point) ? this : nullptr;
    }

    /**
     * Appends this face to `out` if it's currently a reachable Tab stop (see
     * `_is_focusable()`), in tree order. Base case (a plain face has no
     * children): tests only itself. A collection overrides this to recurse
     * into its children afterwards (in insertion order), same as
     * `_find_hit_face()` does for hit-testing, just not reversed, and without
     * gating on whether a face is actually visible on screen: an off-screen
     * (e.g., scrolled-out) but otherwise reachable face is still a valid Tab
     * stop.
     */
    void Face::_collect_tab_stops(std::vector<Face*>& out)
    {
        if (_visible && _enabled && _is_focusable())
            out.push_back(this);
    }



} // namespace fl7::gui
