#ifndef FL7_GUI_FACE_H
#define FL7_GUI_FACE_H

#include "./Style.h"

#include <XiaoLabs/input/Key.h>
#include <XiaoLabs/input/MouseButton.h>

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <optional>
#include <vector>



namespace fl7::gui {



class Shell;
class Collection;



/**
 * A cardinal direction for spatial (arrow key / gamepad d-pad) focus
 * navigation (see `Shell::focus_toward()`).
 */
enum struct ArrowDirection
{
    Up,
    Down,
    Left,
    Right,
}; // enum struct ArrowDirection

/**
 * How a focused face responds to an arrow in a given direction, i.e., whether
 * spatial focus navigation (see `Shell::focus_toward()`) should yield to the face
 * handling that arrow itself instead of moving focus.
 */
enum struct ArrowResponse
{
    /** The arrow is free for focus navigation. */
    Ignore,
    /** The face uses it (e.g., a text caret), but forced navigation (gamepad, Alt+arrow) may override. */
    Soft,
    /** The face needs it (e.g., an open list with somewhere left to go) even under forced navigation. */
    Hard,
}; // enum struct ArrowResponse



/**
 * Which of this face's own parent-relative edges stay anchored as its parent is
 * resized (see `layout::DefaultLayout`), or, reusing the same flags for the
 * perpendicular axis, how it's aligned within the space it's given along that axis
 * (see `layout::BoxLayout`). Combinable (see `operator|`). Meaningless on its own:
 * only consulted by a layout actually attached to this face's parent (see
 * `Container::set_layout()`); with none attached, this face keeps whatever
 * position/size it's explicitly given, regardless of its own anchor.
 */
enum struct Anchor
{
    None        = 0x0,
    Left        = 0x1,
    Right       = 0x2,
    Horizontal  = 0x3,
    Top         = 0x4,
    Bottom      = 0x8,
    Vertical    = 0xc,
    All         = 0xf,
}; // enum struct Anchor

inline Anchor operator|(Anchor a, Anchor b)
{
    return static_cast<Anchor>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline Anchor operator&(Anchor a, Anchor b)
{
    return static_cast<Anchor>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}



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
     * Returns this face's own explicit preferred-size override, independently per
     * component (a component `<= 0` means "not set"; see `get_effective_preferred_size()`
     * for the resolved value a layout would actually use). Default: `{0, 0}` (fully
     * unset).
     */
    ml7::Vector2f get_preferred_size() const { return _preferred_size; }

    /**
     * Sets this face's explicit preferred-size override; see `get_preferred_size()`.
     */
    void set_preferred_size(ml7::Vector2f preferred_size) { _preferred_size = preferred_size; }

    /**
     * Returns this face's effective preferred size: how large a layout (see
     * `layout::Layout`) should try to make it, absent any overriding constraint
     * (available space, a sibling's own minimum size, stretch factors, ...).
     * Combines, independently per component: `get_preferred_size()`, if actually
     * set (a component `<= 0` counts as not set), else this face's own calculated,
     * content-based size (see `_calculate_preferred_size()`; default `{0, 0}`,
     * i.e., no particular opinion; most face types don't have one yet) plus
     * `get_effective_padding()` (the calculated size is content-only; an explicit
     * override is assumed to already have been picked with padding in mind). A
     * plain, content-less face (e.g., `faces::Panel`) therefore reports whatever
     * its own padding alone amounts to (`{0, 0}` by default) unless explicitly
     * told otherwise.
     */
    ml7::Vector2f get_effective_preferred_size() const;

    /**
     * Returns this face's own explicit minimum-size override, independently per
     * component (a component `<= 0` means "not set"; see `get_effective_minimum_size()`
     * for the resolved value a layout would actually use). Default: `{0, 0}` (fully
     * unset).
     */
    ml7::Vector2f get_minimum_size() const { return _minimum_size; }

    /**
     * Sets this face's explicit minimum-size override; see `get_minimum_size()`.
     */
    void set_minimum_size(ml7::Vector2f minimum_size) { _minimum_size = minimum_size; }

    /**
     * Returns this face's effective minimum size: how small a layout (see
     * `layout::Layout`) is allowed to shrink it while distributing space among
     * several children. Combines, independently per component: `get_minimum_size()`,
     * if actually set (a component `<= 0` counts as not set), else this face's own
     * calculated, content-based size (see `_calculate_minimum_size()`) plus
     * `get_effective_padding()` (the calculated size is content-only; an explicit
     * override is assumed to already have been picked with padding in mind). A
     * plain, content-less face (e.g., `faces::Panel`) might report `{0, 0}` unless
     * explicitly told otherwise.
     */
    ml7::Vector2f get_effective_minimum_size() const;

    /**
     * Returns this face's effective padding: its own style's `Style::padding`
     * (see `get_effective_style()`), i.e., the space kept between its own bounds
     * and its actual content, independently on each side.
     */
    Padding get_effective_padding() const { return get_effective_style().padding; }

    /**
     * Returns this face's anchor flags; see `Anchor`. Default: Left|Top, same as
     * having no layout at all: this face keeps whatever position/size it's given,
     * unaffected by its parent being resized.
     */
    Anchor get_anchor() const { return _anchor; }

    /**
     * Sets this face's anchor flags; see `Anchor`.
     */
    void set_anchor(Anchor anchor) { _anchor = anchor; }

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
     * parent chain, netting out each ancestor collection's own get_scroll_offset()
     * along the way, same as Collection::_find_hit_face() and render::DefaultRenderer
     * independently already do while actually hit-testing/drawing this face's
     * descendants: without that, this would return this face's unscrolled, logical
     * position instead of wherever it's actually hit-tested/drawn/visible right now.
     * Not cached (fine at GUI scale, matches the rest of the traversal/hit-testing
     * code).
     */
    ml7::Vector2f get_absolute_position() const;

    /**
     * Returns this face's effective style: its own override if set, else this
     * face's own role resolved against the owning shell's theme (using its own
     * checked/hovered/pressed/focused state, see Theme::State, unless
     * _get_interaction_state_proxy() redirects that part to another face, and
     * unless _requires_own_bounds_for_hover() further narrows a proxied hovered
     * state down to this face's own bounds specifically), else, only for a face
     * with no themed role of its own (see _get_theme_key()), the nearest ancestor's
     * explicit override instead (not that ancestor's own *effective* style, which
     * may itself just be a theme resolution, that's specific to the role of the
     * ancestor, not to the role of this face), so a themed ancestor's override
     * deliberately does NOT reach past a descendant that has its own distinct role
     * (e.g., a Button inside an overridden Panel still looks like a Button), only
     * past anonymous content that has no role to resolve in the first place. Else a
     * default-constructed style as a last resort (no override anywhere up the chain,
     * no theme, or a face type with no themed role).
     */
    Style get_effective_style() const;

    /**
     * Returns this face's optional style override.
     */
    std::optional<Style> get_style_override() const { return _style_override; }

    /**
     * Overrides this face's style (and, transitively, that of any descendant that
     * has neither its own override nor a themed role of its own; see
     * get_effective_style()).
     */
    void set_style_override(const Style& style) { _style_override = style; }

    /**
     * Clears a previously set style override.
     */
    void clear_style_override() { _style_override.reset(); }

    /**
     * Returns this face's explicit Tab-order index (see Shell::update()'s
     * Tab/Shift+Tab handling), or empty if unset.
     */
    std::optional<int> get_tab_order() const { return _tab_order; }

    /**
     * Sets this face's explicit Tab-order index: among the Tab stops (see
     * _is_focusable()) being ordered within a top-level face's subtree, one
     * with an explicit index always comes before one without; two explicit
     * indices compare by their value. Either kind of tie (equal explicit
     * indices, or no explicit index at all) falls back to tree/insertion
     * order. Unset by default: most faces just take their natural position
     * in the tree.
     */
    void set_tab_order(int tab_order) { _tab_order = tab_order; }

    /**
     * Clears a previously set explicit Tab-order index.
     */
    void clear_tab_order() { _tab_order.reset(); }

    /**
     * Returns whether spatial arrow-key/gamepad focus navigation (see
     * Shell::focus_toward()) is currently enabled while this face is the
     * current top-level face. Default: true. Only meaningful on a top-level
     * face: set it false for one that reads the arrows itself for something
     * else entirely (e.g., a HUD overlay steering gameplay), so the shell
     * leaves them to it instead of also moving focus.
     */
    bool get_arrow_navigation_enabled() const { return _arrow_navigation_enabled; }

    /**
     * Sets whether spatial arrow-key/gamepad focus navigation is currently
     * enabled while this face is the current top-level face.
     */
    void set_arrow_navigation_enabled(bool enabled) { _arrow_navigation_enabled = enabled; }



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
     * Returns the face whose checked/hovered/pressed/focused state (see
     * get_effective_style()) should be used to resolve THIS face's theme-based
     * style, or `nullptr` (default) to use this face's own state as usual. For a
     * passive, non-hit-testable decoration owned by an interactive parent (e.g., a
     * CheckBox's little box glyph, itself disabled so clicks/hover always resolve
     * to the CheckBox as a whole; see CheckBox::Box), this lets that decoration
     * still visually track its owner's state without being interactive (or
     * checkable) itself. Doesn't affect _get_theme_key(), which still determines
     * which role's Style is being resolved in the first place.
     */
    virtual const Face* _get_interaction_state_proxy() const { return nullptr; }

    /**
     * Returns whether this face's own hovered state (see get_effective_style())
     * additionally requires the pointer to actually be within its own bounds
     * (see _contains_point()), on top of whatever _get_interaction_state_proxy()
     * says. Default: false, meaning a proxied face fully inherits the proxy's
     * hovered state as-is, same as it does for pressed/focused. A passive
     * decoration that redirects to its owner for pressed/focused (so it still
     * looks "held"/"active" for the whole gesture) but should only actually
     * highlight when the pointer is really over it, not merely somewhere over the
     * owner (e.g., TrackControl's handle vs. the rest of the track), overrides this
     * to true instead.
     */
    virtual bool _requires_own_bounds_for_hover() const { return false; }

    /**
     * Returns whether this face is a valid Tab/Shift+Tab stop (see
     * Shell::update()). Default: false. Override true in a concrete
     * interactive leaf control (Button, CheckBox, RadioButton, TextField,
     * TrackControl, ...): a passive decoration or a plain layout container
     * (Panel, Label, a disabled glyph owned by one of the above, ...) has
     * nothing to type into or activate, so it stays out of the Tab order.
     */
    virtual bool _is_focusable() const { return false; }

    /**
     * Returns whether _on_click() meaningfully does something for this face (see
     * Shell::click_focus()). Default: false. Override true in a concrete face type
     * that overrides _on_click() to actually do something (Button, CheckBox,
     * RadioButton, ...): a face that doesn't (TrackControl, TextField, a plain
     * Panel, ...) has nothing for Enter/Space (or a gamepad's "confirm" button) to
     * trigger.
     */
    virtual bool _is_clickable() const { return false; }

    /**
     * Returns whether this face (while it's the currently focused one) wants
     * the specified arrow for itself, i.e., whether spatial focus navigation
     * (see Shell::focus_toward()) should yield to it instead of moving focus.
     * Default: Ignore (the arrow is always free for navigation). Override in
     * a concrete face type that uses arrows for something of its own (a text
     * caret, a track's handle, a list's selection).
     */
    virtual ArrowResponse _consumes_arrow(ArrowDirection dir) const { return ArrowResponse::Ignore; }

    /**
     * Called with this face's freshly resolved effective text style (part of
     * get_effective_style()'s result), letting a concrete face type enforce a
     * fixed, structural requirement of its own type on it, regardless of
     * whatever a theme (or style override) happens to set, e.g., TextField
     * forcing text_style.preserve_whitespace on, since an editable field's
     * displayed text must always be exactly what was typed, never typeset-
     * trimmed. Default: no-op. NOT a general style-tweaking hook: actual
     * appearance (colors, font, chrome, alignment, ...) stays entirely theme-
     * driven. Most face types have nothing to enforce here at all.
     */
    virtual void _adjust_effective_text_style(fonts::TextStyle& text_style) const {}

    /**
     * Returns this face's own calculated preferred size, based on its own content,
     * for whichever component(s) of `get_effective_preferred_size()` don't have an
     * explicit override (see `set_preferred_size()`). Content-only: `get_effective_padding()`
     * is added on top automatically (see `get_effective_preferred_size()`), not this
     * method's own concern. Default: `{0, 0}` (no particular opinion). Override in a
     * concrete face type whose natural size follows from its own content (e.g.,
     * `faces::Label` from its text).
     */
    virtual ml7::Vector2f _calculate_preferred_size() const { return {}; }

    /**
     * Returns this face's own calculated minimum size, based on its own content,
     * for whichever component(s) of `get_effective_minimum_size()` don't have an
     * explicit override (see `set_minimum_size()`). Content-only: `get_effective_padding()`
     * is added on top automatically, (see `get_effective_minimum_size()`), not this
     * method's own concern. Default: `{0, 0}` (no particular opinion). Override in
     * a concrete face type whose natural size follows from its own content (e.g.,
     * `faces::Label` from its text).
     */
    virtual ml7::Vector2f _calculate_minimum_size() const { return {}; }

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
     * delta and the mouse's current position (both in this face's own local
     * coordinate space, i.e., relative to the absolute screen position, same as
     * `local_position` in _on_mouse_down()), while this face is the "pressed"
     * face and the mouse actually moved. Default: no-op. Override for drag-to-
     * move/drag-to-resize-style behavior (e.g., Frame dragging itself via its
     * title bar, which only needs `delta`) or drag-to-select-style behavior
     * (e.g., TextField extending its selection to wherever the cursor now is,
     * which only needs `local_position`). `delta` is deliberately the OS-synced
     * delta (see Mouse::get_x()), not the raw device delta (Mouse::get_delta_x()):
     * a dragged face should track the visible cursor 1:1, unaffected by whatever
     * the raw device delta vs. OS pointer-acceleration relationship happens to be.
     */
    virtual void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) {}

    /**
     * Called once per frame, with this frame's mouse wheel delta (see
     * Mouse::get_wheel_delta(); one notch/detent is 1), while the wheel actually
     * moved and this face is either the hovered face itself or one of its ancestors
     * that the delta has bubbled up to (see Shell::update()): routed by hover, not
     * focus (wheel scrolling always targets whatever's under the cursor, same as it
     * does in a browser or any other GUI, regardless of which face, if any,
     * currently holds the focus), starting at the hovered face and walking up
     * get_parent() from there, stopping at the first one that actually consumes it.
     *
     * Returns whether this face actually consumed the delta: true stops the
     * bubble-up right here; false lets the next ancestor up try it instead.
     * Default: false (no-op, let it bubble further). A face that's always the
     * meaningful wheel target for its area once reached (e.g., TrackControl, even
     * already clamped at an end) should still return true, so an outer scrollable
     * ancestor (e.g., a ScrollPanel it happens to sit in) doesn't also react to the
     * same notch.
     */
    virtual bool _on_mouse_wheel(int delta) { return false; }

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

    /**
     * Called, while this face is the focused face, once per frame for each key that
     * was pressed down this frame (see Keyboard::is_key_pressed()).
     */
    virtual void _on_key_down(xl7::input::Key key) {}

    /**
     * Called, while this face is the focused face, with this frame's composed text
     * input (see Keyboard::get_text_input()): already keyboard-layout-translated
     * and IME-composed, but NOT filtered for control characters: Enter, Backspace,
     * Escape, Tab etc. are reported as WM_CHAR the same as any other key, so they
     * show up here too (see _on_key_down for handling those instead, and filter
     * them back out here if a plain code point is all you want).
     */
    virtual void _on_text_input(const cl7::u32string& text) {}



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

    /**
     * Appends this face to `out` if it's currently a reachable Tab stop (see
     * _is_focusable()), in tree order. Base case (a plain Face has no
     * children): tests only itself. `Collection` overrides this to recurse
     * into its children afterwards (in insertion order), same as
     * _find_hit_face() does for hit-testing, just not reversed, and without
     * clips_children() gating: an off-screen (e.g. scrolled-out) but
     * otherwise reachable face is still a valid Tab stop.
     */
    virtual void _collect_tab_stops(std::vector<Face*>& out);



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the nearest ancestor's explicit style override, walking up the parent
     * chain, or `nullptr` if none of them have one set.
     */
    const Style* _find_inherited_style_override() const;

    /**
     * Does the actual work of get_effective_style(), minus the final
     * _adjust_effective_text_style() pass.
     */
    Style _resolve_effective_style() const;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning; only set on a top-level face, by Shell::add_face(). */
    Shell* _shell = nullptr;

    /** Non-owning. */
    Face* _parent = nullptr;

    ml7::Vector2f _position;
    ml7::Vector2f _size;

    /** Explicit override; see get_preferred_size()/set_preferred_size(). */
    ml7::Vector2f _preferred_size;
    /** Explicit override; see get_minimum_size()/set_minimum_size(). */
    ml7::Vector2f _minimum_size;

    Anchor _anchor = Anchor::Left | Anchor::Top;

    bool _visible = true;
    bool _enabled = true;

    std::optional<Style> _style_override;

    std::optional<int> _tab_order;

    bool _arrow_navigation_enabled = true;

}; // class Face



} // namespace fl7::gui

#endif // FL7_GUI_FACE_H
