#include "Shell.h"

#include <algorithm>



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
        _mouse_position = mouse_position;
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

        if (const int wheel_delta = _mouse->get_wheel_delta(); wheel_delta != 0)
        {
            // Bubbles up from the hovered face through its ancestors, stopping at
            // the first one that actually consumes it (see Face::_on_mouse_wheel()).
            for (Face* face = _hovered_face; face; face = face->get_parent())
                if (face->_on_mouse_wheel(wheel_delta))
                    break;
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
            _pressed_face->_on_mouse_drag(mouse_position_delta, mouse_position - _pressed_face->get_absolute_position());

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

            // Tab/Shift+Tab is consumed by this shell itself (focus
            // navigation), never forwarded to the focused face's own
            // _on_key_down(); every other key goes through as usual.
            const auto fire_key = [this](Key key) {
                if (key == Key::Tab)
                    _focus_adjacent_tab_stop(_keyboard->is_key_down(Key::LeftShift) || _keyboard->is_key_down(Key::RightShift));
                else
                    _focused_face->_on_key_down(key);
            };

            for (size_t i = 1; i < static_cast<size_t>(Key::COUNT); ++i)
            {
                const Key key = static_cast<Key>(i);
                auto& next_fire_time = _key_repeat_next_fire_times[i];

                if (_keyboard->is_key_pressed(key))
                {
                    fire_key(key);
                    next_fire_time = now + initial_delay;
                }
                else if (!_keyboard->is_key_down(key))
                {
                    next_fire_time.reset();
                }
                else if (next_fire_time && now >= *next_fire_time)
                {
                    fire_key(key);
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



    // #############################################################################
    // Backend Interface (Collection)
    // #############################################################################

    /**
     * Called by Collection::remove_child() just before it actually erases/destroys
     * the specified face (and, transitively, its own descendants, if any): clears
     * whichever of _hovered_face/_pressed_face/_focused_face (if any) falls within
     * that subtree, so none of them is ever left dangling once it's actually gone.
     */
    void Shell::_handle_face_removed(Face* removed)
    {
        const auto is_within_removed_subtree = [removed](Face* face) {
            for (; face; face = face->get_parent())
                if (face == removed)
                    return true;
            return false;
        };

        if (is_within_removed_subtree(_hovered_face))
            _hovered_face = nullptr;
        if (is_within_removed_subtree(_pressed_face))
            _pressed_face = nullptr;
        if (is_within_removed_subtree(_focused_face))
            _focused_face = nullptr;
    }



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
    void Shell::_focus_adjacent_tab_stop(bool backward)
    {
        if (!_focused_face)
            return;

        Face* top_level = _focused_face;
        while (top_level->get_parent())
            top_level = top_level->get_parent();

        std::vector<Face*> tab_stops;
        top_level->_collect_tab_stops(tab_stops);
        if (tab_stops.empty())
            return;

        // An explicit tab_order always comes before an unset one; two
        // explicit ones compare by value. Either kind of tie (equal explicit
        // values, or several unset ones) keeps the tree order _collect_tab_
        // stops() gathered them in, thanks to stable_sort.
        std::ranges::stable_sort(tab_stops, [](const Face* a, const Face* b) {
            const std::optional<int> order_a = a->get_tab_order();
            const std::optional<int> order_b = b->get_tab_order();
            if (order_a.has_value() != order_b.has_value())
                return order_a.has_value();
            return order_a && order_b && *order_a < *order_b;
        });

        const auto it = std::ranges::find(tab_stops, _focused_face);

        size_t next_index;
        if (it == tab_stops.end()) // The focused face (e.g. set via a click) isn't itself a Tab stop.
            next_index = backward ? tab_stops.size() - 1 : 0;
        else
        {
            const size_t current_index = static_cast<size_t>(it - tab_stops.begin());
            next_index = backward
                ? (current_index == 0 ? tab_stops.size() - 1 : current_index - 1)
                : (current_index + 1) % tab_stops.size();
        }

        Face* const next_face = tab_stops[next_index];
        if (next_face == _focused_face)
            return; // Only one Tab stop, and it's already focused.

        _focused_face->_on_focus_lost();
        _focused_face = next_face;
        _focused_face->_on_focus_gained();

        // A key already held before this face gained focus must not
        // immediately fire a spurious repeat on it (see update()).
        _key_repeat_next_fire_times = {};
    }



} // namespace fl7::gui
