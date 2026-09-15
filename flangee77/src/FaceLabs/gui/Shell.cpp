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
    Shell::Shell(render::AbstractRenderer* renderer, xl7::input::Mouse* mouse, xl7::input::Keyboard* keyboard)
        : _renderer(renderer)
        , _mouse(mouse)
        , _keyboard(keyboard)
    {
        assert(_renderer);
        assert(_mouse);
        assert(_keyboard);
    }

    Shell::~Shell() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Refreshes hover/press/focus state from the bound input source's current-frame
     * state. Must be called once per frame, before drawing. Also routes this
     * frame's keyboard text input/key-downs to the focused face, if any: a held key
     * (any of them, not just the ones a particular face happens to care about)
     * auto-repeats after an initial delay, same as text input already does thanks
     * to the OS.
     */
    void Shell::update()
    {
        const ml7::Vector2f mouse_position{static_cast<float>(_mouse->get_x()), static_cast<float>(_mouse->get_y())};
        const ml7::Vector2f mouse_position_delta = _has_previous_mouse_position ? mouse_position - _previous_mouse_position : ml7::Vector2f{};
        _previous_mouse_position = mouse_position;
        _has_previous_mouse_position = true;

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

        if (_mouse->is_button_pressed(MouseButton::Left))
        {
            // Also runs when _hovered_face is null (click on empty space): that's
            // precisely what has to clear a stale focus (e.g., a focused TextField
            // losing its caret again once you click away from it).
            if (_focused_face != _hovered_face)
            {
                if (_focused_face)
                    _focused_face->_on_focus_lost();
                _focused_face = _hovered_face;
                if (_focused_face)
                    _focused_face->_on_focus_gained();

                // A key already held before this face gained focus must not
                // immediately fire a spurious repeat on it.
                _key_repeat_next_fire_times = {};
            }

            if (_hovered_face)
            {
                _pressed_face = _hovered_face;
                _pressed_face->_on_mouse_down(MouseButton::Left, mouse_position - _pressed_face->get_absolute_position());
            }
        }

        if (_pressed_face && (mouse_position_delta.x != 0.0f || mouse_position_delta.y != 0.0f))
            _pressed_face->_on_mouse_drag(mouse_position_delta);

        if (_mouse->is_button_released(MouseButton::Left) && _pressed_face)
        {
            _pressed_face->_on_mouse_up(MouseButton::Left);
            if (_pressed_face == _hovered_face)
                _pressed_face->_on_click(MouseButton::Left);
            _pressed_face = nullptr;
        }

        if (_focused_face)
        {
            const cl7::u32string& text_input = _keyboard->get_text_input();
            if (!text_input.empty())
                _focused_face->_on_text_input(text_input);

            // Mirrors the OS's own auto-repeat for text_input above (which gets
            // it for free from WM_CHAR): an initial delay, then a steady repeat
            // rate, for as long as the key stays held. Unlike text_input, there's
            // no OS repeat signal available for us to piggyback on here (the raw
            // input backend reports physical key transitions only), so this is
            // our own timer, applied uniformly to every key, not just the ones a
            // particular face happens to care about.
            constexpr auto initial_delay = std::chrono::milliseconds(500);
            constexpr auto repeat_interval = std::chrono::milliseconds(40);
            const auto now = std::chrono::steady_clock::now();

            using xl7::input::Key;
            for (size_t i = 1; i < static_cast<size_t>(Key::COUNT); ++i)
            {
                const Key key = static_cast<Key>(i);
                auto& next_fire_time = _key_repeat_next_fire_times[i];

                if (_keyboard->is_key_pressed(key))
                {
                    _focused_face->_on_key_down(key);
                    next_fire_time = now + initial_delay;
                }
                else if (!_keyboard->is_key_down(key))
                {
                    next_fire_time.reset();
                }
                else if (next_fire_time && now >= *next_fire_time)
                {
                    _focused_face->_on_key_down(key);
                    next_fire_time = now + repeat_interval;
                }
            }
        }
    }

    /**
     * Renders all top-level faces (and their children) via the bound renderer.
     * Purely a delegation: this shell knows nothing about how drawing actually
     * happens (or in what order, or whatever), that's entirely up to the renderer.
     */
    void Shell::draw() const
    {
        _renderer->render_faces(_top_level_faces);
    }



} // namespace fl7::gui
