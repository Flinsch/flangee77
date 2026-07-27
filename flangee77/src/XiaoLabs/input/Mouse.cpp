#include "Mouse.h"



namespace xl7::input {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns whether the specified button was tapped (see is_mouse_button_tapped),
     * and the mouse didn't move more than the specified threshold between the
     * button's down and up transitions (i.e., a "click" as opposed to a drag).
     */
    bool Mouse::is_button_clicked(MouseButton button, std::chrono::milliseconds time_window, int move_threshold_px) const
    {
        const MouseButtonState& mb = _buttons[static_cast<size_t>(button)];

        if (!mb.state.is_tapped(_now, time_window))
            return false;

        return _distance_squared(mb.position_at_down, {.x = _x, .y = _y}) <= move_threshold_px * move_threshold_px;
    }

    /**
     * Returns whether the specified button was just clicked (see
     * is_mouse_button_clicked), and a previous click was registered within the
     * specified time window and close to the same position (i.e., a "double click").
     * The previous click is always detected using the default tap window/move
     * threshold, regardless of the parameters passed here or to a concurrent
     * is_mouse_button_clicked call; see _apply() for why.
     */
    bool Mouse::is_button_double_clicked(MouseButton button, std::chrono::milliseconds double_click_time_window, std::chrono::milliseconds click_time_window, int move_threshold_px) const
    {
        const MouseButtonState& mb = _buttons[static_cast<size_t>(button)];

        if (!is_button_clicked(button, click_time_window, move_threshold_px))
            return false;

        if (mb.last_click_time == std::chrono::steady_clock::time_point{})
            return false; // no previous click on record yet

        if (_now - mb.last_click_time > double_click_time_window)
            return false;

        return _distance_squared(mb.last_click_position, mb.position_at_down) <= move_threshold_px * move_threshold_px;
    }



    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed levels/deltas for this frame.
     */
    void Mouse::_apply(const std::array<bool, static_cast<size_t>(MouseButton::COUNT)>& button_values, int delta_x, int delta_y, int wheel_delta, std::chrono::steady_clock::time_point now)
    {
        _x += delta_x;
        _y += delta_y;
        _delta_x = delta_x;
        _delta_y = delta_y;
        _wheel_delta = wheel_delta;
        _now = now;

        for (size_t i = 0; i < button_values.size(); ++i)
        {
            MouseButtonState& mb = _buttons[i];

            // Commit last frame's pending click (if any) so it becomes visible
            // as "the previous click" to this frame's double-click queries,
            // without a click being compared against itself (see the comment
            // on has_pending_click in Mouse.h).
            if (mb.has_pending_click)
            {
                mb.last_click_time = mb.pending_click_time;
                mb.last_click_position = mb.pending_click_position;
                mb.has_pending_click = false;
            }

            mb.state.update(button_values[i], now);

            if (mb.state.is_pressed())
                mb.position_at_down = {.x = _x, .y = _y};

            // Stage this frame's click (if any) for the double-click check,
            // always using the default tap window/move threshold, regardless
            // of whatever parameters a caller might pass to
            // is_mouse_button_clicked()/is_mouse_button_double_clicked(),
            // otherwise the click history a double-click is compared against
            // would depend on which parameters happened to be used for the
            // *previous* query, which isn't a coherent contract.
            if (mb.state.is_tapped(now, DigitalState::DEFAULT_TAP_TIME_WINDOW)
                && _distance_squared(mb.position_at_down, {.x = _x, .y = _y}) <= DEFAULT_CLICK_MOVE_THRESHOLD_PX * DEFAULT_CLICK_MOVE_THRESHOLD_PX)
            {
                mb.pending_click_time = now;
                mb.pending_click_position = mb.position_at_down;
                mb.has_pending_click = true;
            }
        }
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    int Mouse::_distance_squared(const Position& a, const Position& b)
    {
        const int dx = a.x - b.x;
        const int dy = a.y - b.y;
        return dx * dx + dy * dy;
    }



} // namespace xl7::input
