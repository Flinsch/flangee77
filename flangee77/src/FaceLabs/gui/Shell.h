#ifndef FL7_GUI_SHELL_H
#define FL7_GUI_SHELL_H

#include "./Face.h"

#include "./render/AbstractRenderer.h"

#include <XiaoLabs/input.h>
#include <XiaoLabs/input/Keyboard.h>
#include <XiaoLabs/input/Mouse.h>

#include <array>
#include <chrono>
#include <concepts>
#include <memory>
#include <optional>
#include <vector>



namespace fl7::gui {



class Theme;



/**
 * The root/driver of a GUI tree: owns top-level faces, routes input, and drives
 * drawing. Deliberately not tied to any OS window: a shell just needs an input
 * source and a renderer, both handed to it by the caller, so it can equally serve
 * as an in-game HUD, a debug overlay, or (each bound to a different player's
 * individual device) a couch-co-op per-player menu.
 */
class Shell
{

public:
    /**
     * Grants Collection access to notify this shell when one of its children is
     * about to be removed (see Collection::remove_child()), keeping that
     * (backend-facing) capability out of the public query API.
     */
    class Attorney
    {
        static void handle_face_removed(Shell& shell, Face* removed) { shell._handle_face_removed(removed); }
        friend class Collection;
    };



    /**
     * Constructs a shell bound to the specified renderer and input source.
     * Defaults to the aggregate keyboard/mouse for the common single-player case.
     * Pass a specific individual device (e.g., for couch co-op) so multiple shells
     * don't fight over the same aggregate.
     */
    explicit Shell(render::AbstractRenderer* renderer, xl7::input::Mouse* mouse = &xl7::input::mouse(), xl7::input::Keyboard* keyboard = &xl7::input::keyboard());

    Shell(const Shell&) = delete;
    Shell& operator=(const Shell&) = delete;
    Shell(Shell&&) = delete;
    Shell& operator=(Shell&&) = delete;

    ~Shell();



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the keyboard this shell routes input from, e.g. so a focused face can
     * query modifier key state (Shift, Ctrl, ...) from within its own
     * _on_key_down()/_on_mouse_drag() (see TextField's Shift-extended selection and
     * drag-select) instead of every such interaction needing its own dedicated
     * parameter.
     */
    const xl7::input::Keyboard* get_keyboard() const { return _keyboard; }

    /**
     * Returns the (optional) theme faces resolve their style against unless
     * overridden.
     */
    const Theme* get_theme() const { return _theme; }

    /**
     * Sets the (optional) theme faces resolve their style against unless
     * overridden.
     * Non-owning: the caller keeps the theme alive.
     */
    void set_theme(const Theme* theme) { _theme = theme; }

    /**
     * Returns the currently hovered face (the topmost hit under the mouse cursor),
     * or `nullptr`.
     */
    Face* get_hovered_face() const { return _hovered_face; }

    /**
     * Returns the currently pressed (mouse-captured) face, or `nullptr`.
     */
    Face* get_pressed_face() const { return _pressed_face; }

    /**
     * Returns the currently focused face, or `nullptr`.
     */
    Face* get_focused_face() const { return _focused_face; }

    /**
     * Returns the mouse's current absolute (screen) position, as of the most
     * recent update(). Meant for a face whose _requires_own_bounds_for_hover()
     * is true, to check whether the pointer is really within its own bounds,
     * not just somewhere over whatever it's proxying its hovered state from
     * (see Face::get_effective_style()); most faces need get_hovered_face()
     * instead, not this.
     */
    ml7::Vector2f get_mouse_position() const { return _mouse_position; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Constructs a new top-level face of the specified type in place, adds it, and
     * returns a reference to it.
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& add_face(Args&&... args)
    {
        auto face = std::make_unique<TFace>(std::forward<Args>(args)...);
        TFace& ref = *face;
        face->_shell = this;
        _top_level_faces.push_back(std::move(face));
        return ref;
    }

    /**
     * Refreshes hover/press/focus state from the bound input source's current-frame
     * state. Must be called once per frame, before drawing. Also routes this
     * frame's keyboard text input/key-downs to the focused face, if any: a held key
     * (any of them, not just the ones a particular face happens to care about)
     * auto-repeats after an initial delay, same as text input already does thanks
     * to the OS.
     */
    void update();

    /**
     * Moves focus to the nearest focusable face in the given direction
     * within the current top-level face (the focused face's own top-level
     * ancestor, or, with nothing currently focused, the topmost one), by
     * on-screen geometry, unlike the linear Tab/Shift+Tab (see update()).
     * With `force` false, a focused face that consumes the arrow for itself
     * (see Face::_consumes_arrow(), e.g. a text caret) keeps it; with
     * `force` true, only a "hard" consumer (e.g. a list with somewhere left
     * to go) still keeps it. Does nothing if the current top-level face has
     * arrow navigation disabled (see Face::get_arrow_navigation_enabled()),
     * or has no focusable face at all. Called automatically for a plain
     * arrow key press (see update()); exposed publicly so other input (a
     * gamepad's d-pad, say) can drive the same navigation manually.
     */
    void focus_toward(ArrowDirection dir, bool force = false);

    /**
     * Returns whether click_focus() would currently trigger anything: a face is
     * focused, it's clickable, and still enabled/visible (either might have changed
     * since it gained focus, without it losing focus over that alone). Exposed so a
     * caller (a gamepad's "confirm" button, say) can tell in advance whether
     * pressing it would do something.
     */
    bool can_click_focus() const;

    /**
     * Triggers the currently focused face as if it had just been clicked, if
     * can_click_focus(); returns whether it actually did. Called automatically for
     * Enter/Space (see update()). Exposed publicly so other input (a gamepad's
     * "confirm" button, say) can trigger the same activation manually.
     */
    bool click_focus();

    /**
     * Renders all top-level faces (and their children) via the bound renderer.
     * Purely a delegation: this shell knows nothing about how drawing actually
     * happens (or in what order, or whatever), that's entirely up to the renderer.
     */
    void draw() const;



private:

    // #############################################################################
    // Backend Interface (Collection)
    // #############################################################################

    /**
     * Called by Collection::remove_child() just before it actually erases/destroys
     * the specified face (and, transitively, its own descendants, if any): clears
     * whichever of _hovered_face/_pressed_face/_focused_face (if any) falls within
     * that subtree, so none of them is ever left dangling once it's actually gone.
     */
    void _handle_face_removed(Face* removed);



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Moves focus to the next (or, if `backward`, previous) Tab stop (see
     * Face::_is_focusable()/get_tab_order()), scoped to the currently focused
     * face's own top-level face's subtree, wrapping around at either end.
     * No-op if nothing is currently focused (there's no "current" top-level
     * face to scope to otherwise, see update()'s Tab handling) or if that
     * subtree has no Tab stop at all.
     */
    void _focus_adjacent_tab_stop(bool backward);

    /**
     * Moves the specified top-level face (which must actually currently be one) to
     * the back of _top_level_faces, i.e., the front of the Z-order (see update(),
     * hit-tested/drawn last, same as a newly added one would be): the usual "bring
     * the clicked-into window to the front" behavior. A no-op if it's already there.
     */
    void _bring_to_front(Face* top_level);

    /**
     * Returns the current top-level face: the focused face's own top-level
     * ancestor if something is currently focused, else the topmost
     * (last-added) top-level face, or `nullptr` if there are none at all.
     */
    Face* _current_top_level_face() const;

    /**
     * Returns the nearest focusable face in the given direction from the
     * currently focused face, among `top_level`'s own Tab stops (see
     * Face::_collect_tab_stops()), by on-screen geometry, or `nullptr` if
     * there's no usable candidate. Without a usable starting point (no
     * current focus, or an invisible one), picks the outermost candidate in
     * that direction instead (topmost for Up, bottommost for Down, and so
     * on), so entering a top-level face for the first time lands on an
     * intuitive face.
     */
    Face* _find_face_toward(Face* top_level, ArrowDirection dir, bool force) const;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning. */
    render::AbstractRenderer* _renderer;
    /** Non-owning. */
    xl7::input::Mouse* _mouse;
    /** Non-owning. */
    xl7::input::Keyboard* _keyboard;

    /** Non-owning. */
    const Theme* _theme = nullptr;

    std::vector<std::unique_ptr<Face>> _top_level_faces;

    /** Non-owning: the topmost face currently under the mouse cursor. */
    Face* _hovered_face = nullptr;
    /** Non-owning: the face that received mouse-down, until mouse-up. */
    Face* _pressed_face = nullptr;
    /** Non-owning: the face that is the focus target. */
    Face* _focused_face = nullptr;

    /** The mouse's current absolute (screen) position, as of the most recent update(). */
    ml7::Vector2f _mouse_position;

    /** The mouse position as of the previous update, for computing the delta for mouse drags. */
    ml7::Vector2f _previous_mouse_position;
    /** Whether the previous mouse position has actually been set yet (guards first update's delta against a spurious jump). */
    bool _has_previous_mouse_position = false;

    /**
     * Per-key next scheduled auto-repeat fire time for _on_key_down (see update()),
     * while that key is being held and repeated; unset otherwise. Indexed the same
     * way as Keyboard's own per-key state. Cleared whenever _focused_face changes,
     * so a key already held before a face gained focus never fires a spurious
     * immediate repeat on it.
     */
    std::array<std::optional<std::chrono::steady_clock::time_point>, static_cast<size_t>(xl7::input::Key::COUNT)> _key_repeat_next_fire_times;

}; // class Shell



} // namespace fl7::gui

#endif // FL7_GUI_SHELL_H
