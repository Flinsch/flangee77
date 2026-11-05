#include "Shell.h"

#include <algorithm>
#include <iterator>



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
            // Raises whichever top-level face was actually hit (if any) to the
            // front of _top_level_faces, same as any other GUI bringing a
            // clicked-into window to the front of a stack of overlapping ones.
            // Affects only the NEXT hit-test (see above, already done for this
            // very frame), never this frame's own hover/press/focus.
            if (_hovered_face)
            {
                Face* top_level = _hovered_face;
                while (top_level->get_parent())
                    top_level = top_level->get_parent();
                _bring_to_front(top_level);
            }

            // Also runs when _hovered_face is null (click on empty space): that's
            // precisely what has to clear a stale focus (e.g., a focused TextField
            // losing its caret again once you click away from it).
            _change_focus(_hovered_face);

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

            // Tab/Shift+Tab and Enter/Space are consumed by this shell itself
            // (focus navigation/activation), never forwarded to the focused
            // face's own _on_key_down() (text input, e.g., a literal space typed
            // into a TextField, still goes through separately, see the text_input
            // handling above). A plain arrow key triggers focus_toward() first,
            // querying _consumes_arrow() against whatever state the focused face is
            // still in from BEFORE this keypress (e.g., a ListBox's current
            // selection): only once that's decided does the key still go through to
            // _on_key_down() as usual (e.g., TextField's caret, or that same ListBox
            // selection actually moving), dispatched to whichever face was focused
            // at the start, regardless of whether focus_toward() just moved on.
            const auto fire_key = [this](Key key) {
                if (key == Key::Tab)
                {
                    _focus_adjacent_tab_stop(_keyboard->is_key_down(Key::LeftShift) || _keyboard->is_key_down(Key::RightShift));
                    return;
                }

                if (key == Key::Enter || key == Key::Space)
                {
                    click_focus();
                    return;
                }

                Face* const original_focus = _focused_face;

                const bool force = _keyboard->is_key_down(Key::LeftAlt) || _keyboard->is_key_down(Key::RightAlt);
                switch (key)
                {
                case Key::Up:    focus_toward(ArrowDirection::Up, force);    break;
                case Key::Down:  focus_toward(ArrowDirection::Down, force);  break;
                case Key::Left:  focus_toward(ArrowDirection::Left, force);  break;
                case Key::Right: focus_toward(ArrowDirection::Right, force); break;
                default: break;
                }

                if (original_focus)
                    original_focus->_on_key_down(key);
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
    void Shell::focus_toward(ArrowDirection dir, bool force)
    {
        Face* const top_level = _current_top_level_face();
        if (!top_level || !top_level->get_arrow_navigation_enabled())
            return;

        Face* const face = _find_face_toward(top_level, dir, force);
        if (!face)
            return;

        _change_focus(face);
    }

    /**
     * Returns whether click_focus() would currently trigger anything: a face is
     * focused, it's clickable, and still enabled/visible (either might have changed
     * since it gained focus, without it losing focus over that alone). Exposed so a
     * caller (a gamepad's "confirm" button, say) can tell in advance whether
     * pressing it would do something.
     */
    bool Shell::can_click_focus() const
    {
        return _focused_face && _focused_face->_is_clickable() && _focused_face->is_enabled() && _focused_face->is_visible();
    }

    /**
     * Triggers the currently focused face as if it had just been clicked, if
     * can_click_focus(); returns whether it actually did. Called automatically for
     * Enter/Space (see update()). Exposed publicly so other input (a gamepad's
     * "confirm" button, say) can trigger the same activation manually.
     */
    bool Shell::click_focus()
    {
        if (!can_click_focus())
            return false;

        _focused_face->_on_click(xl7::input::MouseButton::Left);
        return true;
    }

    /**
     * Moves focus directly to the specified face (or, with `nullptr`, clears it),
     * the same way a click or Tab/arrow navigation would: fires
     * _on_focus_lost()/_on_focus_gained() as needed, a no-op if `face` is already
     * the focused face. Exposed for a face that needs to hand focus back to itself
     * (or elsewhere) of its own accord, outside of any direct click on it (e.g.,
     * faces::DropDown, once its popup list closes: actually clicking into the
     * popup unavoidably focuses it first instead, see update()'s mouse handling).
     */
    void Shell::set_focus(Face* face)
    {
        _change_focus(face);
    }

    /**
     * Registers `popup` (which must already be one of this shell's own top-level
     * faces, see add_face()) as a popup owned by `owner`, bringing it to the front
     * of the Z-order (see update()) and keeping it there every time this is called
     * again, e.g. on every re-open: without this, a popup that was only ever
     * created once and hidden/shown again afterwards (Shell has no way to remove a
     * top-level face at all, see close_popup()) would stay wherever it first ended
     * up in the Z-order, even once some unrelated face has since been brought to
     * front ahead of it.
     *
     * Also makes this shell close the popup again on its own (see close_popup()),
     * the moment focus moves to a face that is neither the popup nor `owner`, nor
     * a descendant of either (a plain click elsewhere, Tab/arrow navigation moving
     * on, ...): the common "clicking/tabbing away closes the open dropdown/menu/
     * ..." behavior, implemented once here instead of by every individual popup-
     * opening face. A click into the popup's own content, or back onto `owner`
     * itself (e.g., to toggle it closed again), is deliberately NOT treated as
     * "elsewhere", so it doesn't get auto-closed out from under that interaction.
     */
    void Shell::open_popup(Face* popup, Face* owner)
    {
        assert(popup);
        _bring_to_front(popup);

        const auto it = std::ranges::find_if(_popups, [popup](const PopupEntry& entry) { return entry.popup == popup; });
        if (it != _popups.end())
            it->owner = owner;
        else
            _popups.push_back({popup, owner});
    }

    /**
     * Unregisters and hides (set_visible(false)) the given popup, if currently
     * registered (see open_popup()); a no-op otherwise. Called automatically once
     * focus moves away (see open_popup()), but just as well callable directly for
     * an explicit close (e.g., Escape, or the opening face toggling it closed again).
     */
    void Shell::close_popup(Face* popup)
    {
        const auto it = std::ranges::find_if(_popups, [popup](const PopupEntry& entry) { return entry.popup == popup; });
        if (it == _popups.end())
            return;

        popup->set_visible(false);
        _popups.erase(it);
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

        // A popup's owner can be removed (destroyed) while its popup is still
        // open (e.g., a DropDown's own enclosing Panel being torn down): closes
        // any such now-ownerless popup right away, rather than leaving it open
        // indefinitely (a popup itself, always a top-level face, can never be
        // "removed" this way to begin with, see open_popup()).
        std::erase_if(_popups, [&](const PopupEntry& entry) {
            if (!is_within_removed_subtree(entry.owner))
                return false;
            entry.popup->set_visible(false);
            return true;
        });
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Changes _focused_face to the specified face (or `nullptr`), firing
     * _on_focus_lost()/_on_focus_gained() as needed, and resetting the key-repeat
     * timers so a key already held before the new face gained focus doesn't
     * immediately fire a spurious repeat on it. A no-op if `face` is already the
     * focused face. The shared implementation behind update()'s mouse-press
     * handling, focus_toward(), _focus_adjacent_tab_stop(), and set_focus().
     */
    void Shell::_change_focus(Face* face)
    {
        if (face == _focused_face)
            return;

        if (_focused_face)
            _focused_face->_on_focus_lost();
        _focused_face = face;
        if (_focused_face)
            _focused_face->_on_focus_gained();

        // A key already held before this face gained focus must not
        // immediately fire a spurious repeat on it.
        _key_repeat_next_fire_times = {};

        // Closes every registered popup (see open_popup()) that the newly
        // focused face falls within neither the subtree of, nor the subtree of
        // its own owner: a plain click/Tab/arrow move away from both closes it,
        // same as landing back within the popup's own content, or back on its
        // owner (e.g. to toggle it closed again), does not.
        const auto is_within = [](Face* ancestor, Face* f) {
            for (; f; f = f->get_parent())
                if (f == ancestor)
                    return true;
            return false;
        };
        std::erase_if(_popups, [&](const PopupEntry& entry) {
            if (is_within(entry.popup, face) || is_within(entry.owner, face))
                return false;
            entry.popup->set_visible(false);
            return true;
        });
    }

    /**
     * Moves the specified top-level face (which must actually currently be one) to
     * the back of _top_level_faces, i.e., the front of the Z-order (see update(),
     * hit-tested/drawn last, same as a newly added one would be): the usual "bring
     * the clicked-into window to the front" behavior. A no-op if it's already there.
     *
     * Also raises, the same way, any registered popup (see open_popup()) whose
     * owner lies within `top_level`'s own subtree: without this, raising a face
     * whose own popup happens to currently sit further back (e.g., some other face
     * having been raised in between) would leave that popup stranded visually
     * behind it, for however long it still takes to actually get closed (its
     * owner's own click handling, e.g., toggling it closed again, only takes
     * effect once that press is released, see update()'s mouse handling): purely a
     * Z-order correction, never closing or otherwise touching the popup itself.
     * Raising the popup itself needs no such chase the other way around.
     */
    void Shell::_bring_to_front(Face* top_level)
    {
        const auto raise = [this](Face* face) {
            const auto it = std::ranges::find_if(_top_level_faces, [face](const std::unique_ptr<Face>& f) {
                return f.get() == face;
            });
            assert(it != _top_level_faces.end());

            if (it == std::prev(_top_level_faces.end()))
                return; // Already at the front.

            std::unique_ptr<Face> moved = std::move(*it);
            _top_level_faces.erase(it);
            _top_level_faces.push_back(std::move(moved));
        };

        raise(top_level);

        for (const PopupEntry& entry : _popups)
        {
            if (entry.popup == top_level)
                continue;

            for (Face* f = entry.owner; f; f = f->get_parent())
            {
                if (f == top_level)
                {
                    raise(entry.popup);
                    break;
                }
            }
        }
    }

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

        _change_focus(tab_stops[next_index]); // A no-op if it's the only Tab stop, and it's already focused.
    }

    /**
     * Returns the current top-level face: the focused face's own top-level
     * ancestor if something is currently focused, else the topmost
     * (last-added) top-level face, or `nullptr` if there are none at all.
     */
    Face* Shell::_current_top_level_face() const
    {
        if (_focused_face)
        {
            Face* top_level = _focused_face;
            while (top_level->get_parent())
                top_level = top_level->get_parent();
            return top_level;
        }

        return _top_level_faces.empty() ? nullptr : _top_level_faces.back().get();
    }

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
    Face* Shell::_find_face_toward(Face* top_level, ArrowDirection dir, bool force) const
    {
        std::vector<Face*> candidates;
        top_level->_collect_tab_stops(candidates);
        if (candidates.empty())
            return nullptr;

        const bool focus_is_anchor = _focused_face && _focused_face->is_visible();
        const bool focus_is_navigable = focus_is_anchor && std::ranges::find(candidates, _focused_face) != candidates.end();

        // Yield to a navigable focused face that wants this arrow for
        // itself: a hard consumer always, a soft one unless forced (e.g., a
        // text caret).
        if (focus_is_navigable)
        {
            const ArrowResponse response = _focused_face->_consumes_arrow(dir);
            if (response == ArrowResponse::Hard)
                return nullptr;
            if (response == ArrowResponse::Soft && !force)
                return nullptr;
        }

        // Without a usable starting point, spatial navigation enters the
        // top-level face at the outermost candidate in the pressed
        // direction: a direct scan rather than repeatedly stepping, so it
        // reliably reaches the far end regardless of the layout in between.
        if (!focus_is_anchor)
        {
            Face* best = nullptr;
            float best_edge = 0.0f;
            for (Face* face : candidates)
            {
                const ml7::Vector2f center = face->get_absolute_position() + face->get_size() * 0.5f;
                float edge = 0.0f;
                switch (dir)
                {
                case ArrowDirection::Up:    edge = -center.y; break;
                case ArrowDirection::Down:  edge =  center.y; break;
                case ArrowDirection::Left:  edge = -center.x; break;
                case ArrowDirection::Right: edge =  center.x; break;
                }
                if (!best || edge > best_edge)
                {
                    best = face;
                    best_edge = edge;
                }
            }
            return best;
        }

        // The orthogonal offset is weighted (quadratically, since both terms
        // are squared) so that among candidates at a similar range the
        // better-aligned one wins: this is the angular preference. The beam
        // test below matters more, though.
        constexpr float ortho_weight = 2.0f;
        constexpr float ortho_weight_squared = ortho_weight * ortho_weight;

        const ml7::Vector2f from_pos = _focused_face->get_absolute_position();
        const ml7::Vector2f from_size = _focused_face->get_size();
        const ml7::Vector2f from_center = from_pos + from_size * 0.5f;

        Face* best = nullptr;
        bool best_in_beam = false;
        float best_score = 0.0f;

        for (Face* face : candidates)
        {
            if (face == _focused_face)
                continue;

            const ml7::Vector2f to_pos = face->get_absolute_position();
            const ml7::Vector2f to_size = face->get_size();
            const ml7::Vector2f to_center = to_pos + to_size * 0.5f;
            const float dx = to_center.x - from_center.x;
            const float dy = to_center.y - from_center.y;

            float major = 0.0f;   // Distance along the pressed direction (must be > 0).
            float minor = 0.0f;   // Offset on the orthogonal axis.
            bool in_beam = false; // Does the candidate overlap the source's cross-section?
            switch (dir)
            {
            case ArrowDirection::Up:
                if (dy >= 0.0f) continue;
                major = -dy; minor = dx < 0.0f ? -dx : dx;
                in_beam = to_pos.x < from_pos.x + from_size.x && to_pos.x + to_size.x > from_pos.x;
                break;
            case ArrowDirection::Down:
                if (dy <= 0.0f) continue;
                major = dy; minor = dx < 0.0f ? -dx : dx;
                in_beam = to_pos.x < from_pos.x + from_size.x && to_pos.x + to_size.x > from_pos.x;
                break;
            case ArrowDirection::Left:
                if (dx >= 0.0f) continue;
                major = -dx; minor = dy < 0.0f ? -dy : dy;
                in_beam = to_pos.y < from_pos.y + from_size.y && to_pos.y + to_size.y > from_pos.y;
                break;
            case ArrowDirection::Right:
                if (dx <= 0.0f) continue;
                major = dx; minor = dy < 0.0f ? -dy : dy;
                in_beam = to_pos.y < from_pos.y + from_size.y && to_pos.y + to_size.y > from_pos.y;
                break;
            }

            const float score = major * major + minor * minor * ortho_weight_squared;

            // A candidate that overlaps the source's cross-section ("in
            // beam", a true neighbour in that direction) always wins over
            // any that does not, so a directly adjacent element beats a
            // closer but diagonal one. The weighted distance orders
            // candidates within the same group.
            const bool better = !best
                || (in_beam && !best_in_beam)
                || (in_beam == best_in_beam && score < best_score);
            if (better)
            {
                best = face;
                best_in_beam = in_beam;
                best_score = score;
            }
        }

        return best;
    }



} // namespace fl7::gui
