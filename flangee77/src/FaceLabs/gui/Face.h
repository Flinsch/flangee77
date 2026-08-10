#ifndef FL7_GUI_FACE_H
#define FL7_GUI_FACE_H

#include "./Style.h"

#include <XiaoLabs/input/MouseButton.h>

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <optional>



namespace fl7::gui {



class Shell;
class Collection;



/**
 * Base class for all GUI elements ("faces"): position/size (relative to its parent),
 * visibility/enabled state, style resolution, and the input-event hooks. Doesn't
 * own children itself (see `Collection` for that), so a plain face is a leaf.
 * Absolute (screen) position is computed on the fly during hit-testing/rendering
 * traversal, not cached (fine at GUI scale). Knows nothing about drawing/rendering
 * itself, that's entirely up to implementations of `render::AbstractRenderer`.
 */
class Face
{
    friend class Shell;
    friend class Collection;

public:
    Face() = default;

    Face(const Face&) = delete;
    Face& operator=(const Face&) = delete;
    Face(Face&&) = delete;
    Face& operator=(Face&&) = delete;

    virtual ~Face() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the shell this face (transitively) belongs to, or `nullptr` if it
     * isn't (yet) part of one. Only ever actually set (by Shell::add_face()) on a
     * top-level face, so this walks up the parent chain to find it. Not cached
     * (fine at GUI scale.
     */
    Shell* get_shell() const;

    /**
     * Returns this face's parent, or `nullptr` for a top-level Face.
     */
    Face* get_parent() const { return _parent; }

    /**
     * Returns this Face's position, relative to its parent (or, for a top-level
     * face, relative to the shell's origin).
     */
    ml7::Vector2f get_position() const { return _position; }

    /**
     * Sets this face's position, relative to its parent.
     */
    void set_position(ml7::Vector2f position) { _position = position; }

    /**
     * Returns this face's size.
     */
    ml7::Vector2f get_size() const { return _size; }

    /**
     * Sets this face's size.
     */
    void set_size(ml7::Vector2f size);

    /**
     * Returns whether this face is currently visible (drawn, and eligible for
     * hit-testing). An invisible face's children are neither drawn nor hit-tested
     * either.
     */
    bool is_visible() const { return _visible; }

    /**
     * Sets whether this face is currently visible (drawn, and eligible for
     * hit-testing). An invisible face's children are neither drawn nor hit-tested
     * either.
     */
    void set_visible(bool visible) { _visible = visible; }

    /**
     * Returns whether this face currently participates in hit-testing (independent
     * of visibility).
     */
    bool is_enabled() const { return _enabled; }

    /**
     * Sets whether this face currently participates in hit-testing (independent
     * of visibility).
     */
    void set_enabled(bool enabled) { _enabled = enabled; }

    /**
     * Returns this face's absolute (screen) position, computed by walking up the
     * parent chain. Not cached (fine at GUI scale, matches the rest of the
     * traversal/hit-testing code).
     */
    ml7::Vector2f get_absolute_position() const;

    /**
     * Returns this face's effective style: its own override if set, else the
     * nearest ancestor's explicit override (not that ancestor's own *effective*
     * style, which may itself just be a theme resolution, that's specific to the
     * role of the ancestor, not to the role of this face), else this face's own
     * role resolved against the owning shell's theme, else a default-constructed
     * style as a last resort (no override anywhere up the chain, no theme, or a
     * face type with no themed role).
     */
    Style get_effective_style() const;

    /**
     * Returns this face's optional style override.
     */
    std::optional<Style> get_style_override() const { return _style_override; }

    /**
     * Overrides this face's style (and, transitively, that of any descendant that
     * doesn't have its own override).
     */
    void set_style_override(const Style& style) { _style_override = style; }

    /**
     * Clears a previously set style override.
     */
    void clear_style_override() { _style_override.reset(); }



protected:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns this face's role/key for theme-based style resolution (see
     * get_effective_style()), e.g., "window", "button", etc. Default: empty,
     * meaning this face type doesn't participate in theming (falls straight through
     * to the ordinary inherited-override/last-resort-default style). Override in
     * concrete types that do.
     */
    virtual cl7::u8string_view _get_theme_key() const { return {}; }

    /**
     * Returns whether the specified point, in this face's own local coordinate
     * space (i.e., relative to this face's own absolute position), lies within it.
     * Default: an axis-aligned rect test against `get_size`. Override for a
     * non-rectangular hit shape.
     */
    virtual bool _contains_point(ml7::Vector2f local_point) const;

    /**
     * Called after this face's size has actually changed (see set_size), with both
     * the old and the new size. Default: no-op. Override to react to a size change
     * (e.g., Window keeping its internal content area in sync).
     */
    virtual void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) {}

    /**
     * Called when this face becomes the hovered face (the topmost hit under the
     * mouse cursor).
     */
    virtual void _on_mouse_enter() {}

    /**
     * Called when this face stops being the hovered Face.
     */
    virtual void _on_mouse_leave() {}

    /**
     * Called when the specified mouse button was pressed down while this face was
     * the hovered face (this face becomes the "pressed" face, i.e., mouse-captured,
     * until the button is released), with the press position in this face's own
     * local coordinate space (i.e., relative to the absolute screen position).
     */
    virtual void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) {}

    /**
     * Called when the specified mouse button was released while this face was the
     * "pressed" face (regardless of whether the cursor is still over it; see
     * `on_click` for the "still over it" case).
     */
    virtual void _on_mouse_up(xl7::input::MouseButton button) {}

    /**
     * Called once per frame, with this frame's real (OS-synced) mouse movement
     * delta, while this face is the "pressed" face and the mouse actually moved.
     * Default: no-op. Override for drag-to-move/drag-to-resize-style behavior
     * (e.g., frame dragging itself via its title bar). Deliberately the OS-synced
     * delta (see Mouse::get_x()), not the raw device delta (Mouse::get_delta_x()):
     * a dragged face should track the visible cursor 1:1, unaffected by whatever
     * the raw device delta vs. OS pointer-acceleration relationship happens to be.
     */
    virtual void _on_mouse_drag(ml7::Vector2f delta) {}

    /**
     * Called when the specified mouse button was pressed down and released again
     * while this face was continuously the hovered face (i.e., a "click": press and
     * release on the same face, not just anywhere).
     */
    virtual void _on_click(xl7::input::MouseButton button) {}

    /**
     * Called when this face becomes the focused face.
     */
    virtual void _on_focus_gained() {}

    /**
     * Called when this face stops being the focused face.
     */
    virtual void _on_focus_lost() {}



    // #############################################################################
    // Backend Interface (Shell / Collection)
    // #############################################################################

    /**
     * Finds the topmost face containing the given absolute (screen) point.
     * Base case (a plain Face has no children): tests only itself, returning
     * `nullptr` if invisible/disabled or not hit. `Collection` overrides this
     * to check its children first (topmost/most-recently-added first), only
     * falling back to this same self-test if none of them are hit.
     */
    virtual Face* _find_hit_face(ml7::Vector2f screen_point, ml7::Vector2f parent_absolute_position);



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the nearest ancestor's explicit style override, walking up the parent
     * chain, or `nullptr` if none of them have one set.
     */
    const Style* _find_inherited_style_override() const;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning; only set on a top-level face, by Shell::add_face(). */
    Shell* _shell = nullptr;

    /** Non-owning. */
    Face* _parent = nullptr;

    ml7::Vector2f _position;
    ml7::Vector2f _size;

    bool _visible = true;
    bool _enabled = true;

    std::optional<Style> _style_override;

}; // class Face



} // namespace fl7::gui

#endif // FL7_GUI_FACE_H
