#ifndef FL7_GUI_SHELL_H
#define FL7_GUI_SHELL_H

#include "./Face.h"

#include "./render/AbstractRenderer.h"

#include <XiaoLabs/input.h>
#include <XiaoLabs/input/Keyboard.h>
#include <XiaoLabs/input/Mouse.h>

#include <concepts>
#include <memory>
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
     * state. Must be called once per frame, before drawing.
     */
    void update();

    /**
     * Renders all top-level faces (and their children) via the bound renderer.
     * Purely a delegation: this shell knows nothing about how drawing actually
     * happens (or in what order, or whatever), that's entirely up to the renderer.
     */
    void draw() const;



private:

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

    /** The mouse position as of the previous update, for computing the delta for mouse drags. */
    ml7::Vector2f _previous_mouse_position;
    /** Whether the previous mouse position has actually been set yet (guards first update's delta against a spurious jump). */
    bool _has_previous_mouse_position = false;

}; // class Shell



} // namespace fl7::gui

#endif // FL7_GUI_SHELL_H
