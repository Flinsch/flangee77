#include "Shell.h"



namespace fl7::gui {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Constructs a shell bound to the specified renderer and input source.
     * Defaults to the aggregate keyboard/mouse for the common single-player case.
     * Pass a specific individual device (e.g., for couch co-op) so multiple shells
     * don't fight over the same aggregate.
     */
    Shell::Shell(render::AbstractRenderer& renderer, xl7::input::Mouse& mouse, xl7::input::Keyboard& keyboard)
        : _renderer(renderer)
        , _mouse(mouse)
        , _keyboard(keyboard)
    {
    }

    Shell::~Shell() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Refreshes hover/press/focus state from the bound input source's current-frame
     * state. Must be called once per frame, before drawing.
     */
    void Shell::update()
    {
        const ml7::Vector2f mouse_position{static_cast<float>(_mouse.get_x()), static_cast<float>(_mouse.get_y())};

        Face* hit = nullptr;
        for (auto it = _top_level_faces.rbegin(); it != _top_level_faces.rend() && !hit; ++it)
            hit = (*it)->_find_hit_face(mouse_position, {});

        if (hit != _hovered_face)
        {
            if (_hovered_face)
                _hovered_face->_on_mouse_leave();
            _hovered_face = hit;
            if (_hovered_face)
                _hovered_face->_on_mouse_enter();
        }

        using xl7::input::MouseButton;

        if (_mouse.is_button_pressed(MouseButton::Left) && _hovered_face)
        {
            _pressed_face = _hovered_face;

            if (_focused_face != _pressed_face)
            {
                if (_focused_face)
                    _focused_face->_on_focus_lost();
                _focused_face = _pressed_face;
                _focused_face->_on_focus_gained();
            }

            _pressed_face->_on_mouse_down(MouseButton::Left);
        }

        if (_mouse.is_button_released(MouseButton::Left) && _pressed_face)
        {
            _pressed_face->_on_mouse_up(MouseButton::Left);
            if (_pressed_face == _hovered_face)
                _pressed_face->_on_click(MouseButton::Left);
            _pressed_face = nullptr;
        }
    }

    /**
     * Renders all top-level faces (and their children) via the bound renderer.
     * Purely a delegation: this shell knows nothing about how drawing actually
     * happens (or in what order, or whatever), that's entirely up to the renderer.
     */
    void Shell::draw() const
    {
        _renderer.render_faces(_top_level_faces);
    }



} // namespace fl7::gui
