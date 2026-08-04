#ifndef XL7_INPUT_MOUSE_H
#define XL7_INPUT_MOUSE_H

#include "./DigitalState.h"
#include "./MouseButton.h"

#include <array>
#include <chrono>



namespace xl7::input {



/**
 * The default maximum distance (in pixels) the mouse may move between a
 * button's down and up transitions for the cycle to still count as a "click"
 * (as opposed to a drag). Callers may override this per query.
 */
inline constexpr int DEFAULT_CLICK_MOVE_THRESHOLD_PX = 4;

/**
 * The default maximum time between two clicks for them to count as a "double
 * click". Callers may override this per query.
 */
inline constexpr auto DEFAULT_DOUBLE_CLICK_WINDOW = std::chrono::milliseconds(400);

/**
 * Represents a single mouse's state (or the aggregate of all connected mice,
 * which is fed the same way, just with OR'd button values and summed
 * movement/wheel deltas instead of a single physical device's raw signals).
 */
class Mouse
{

public:
    /**
     * Grants KeyboardMouseSystem exclusive access to feed new state into a
     * Mouse instance, keeping that (backend-facing) capability out of the
     * public query API.
     */
    class Attorney
    {
        static void apply(Mouse& mouse, const std::array<bool, static_cast<size_t>(MouseButton::COUNT)>& button_values, int delta_x, int delta_y, int wheel_delta, std::chrono::steady_clock::time_point now) { mouse._apply(button_values, delta_x, delta_y, wheel_delta, now); }
        static void apply_cursor_position(Mouse& mouse, int x, int y) { mouse._apply_cursor_position(x, y); }
        friend class KeyboardMouseSystem;
    };



    Mouse() = default;

    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(Mouse&&) = delete;

    ~Mouse() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the state of the specified button.
     */
    const DigitalState& get_button(MouseButton button) const { return _buttons[static_cast<size_t>(button)].state; }

    /**
     * Returns whether the specified button is currently held down.
     */
    bool is_button_down(MouseButton button) const { return _buttons[static_cast<size_t>(button)].state.is_down(); }

    /**
     * Returns whether the specified button was pressed down during this frame.
     */
    bool is_button_pressed(MouseButton button) const { return _buttons[static_cast<size_t>(button)].state.is_pressed(); }

    /**
     * Returns whether the specified button was released during this frame.
     */
    bool is_button_released(MouseButton button) const { return _buttons[static_cast<size_t>(button)].state.is_released(); }

    /**
     * Returns whether the specified button was released during this frame, and the
     * down/up cycle it completed fit within the specified time window.
     */
    bool is_button_tapped(MouseButton button, std::chrono::milliseconds time_window = DigitalState::DEFAULT_TAP_TIME_WINDOW) const { return _buttons[static_cast<size_t>(button)].state.is_tapped(_now, time_window); }

    /**
     * Returns whether the specified button was tapped (see is_mouse_button_tapped),
     * and the mouse didn't move more than the specified threshold between the
     * button's down and up transitions (i.e., a "click" as opposed to a drag).
     */
    bool is_button_clicked(MouseButton button, std::chrono::milliseconds time_window = DigitalState::DEFAULT_TAP_TIME_WINDOW, int move_threshold_px = DEFAULT_CLICK_MOVE_THRESHOLD_PX) const;

    /**
     * Returns whether the specified button was just clicked (see
     * is_mouse_button_clicked), and a previous click was registered within the
     * specified time window and close to the same position (i.e., a "double click").
     * The previous click is always detected using the default tap window/move
     * threshold, regardless of the parameters passed here or to a concurrent
     * is_mouse_button_clicked call; see _apply() for why.
     */
    bool is_button_double_clicked(MouseButton button, std::chrono::milliseconds double_click_time_window = DEFAULT_DOUBLE_CLICK_WINDOW, std::chrono::milliseconds time_window = DigitalState::DEFAULT_TAP_TIME_WINDOW, int move_threshold_px = DEFAULT_CLICK_MOVE_THRESHOLD_PX) const;

    /**
     * Returns the x coordinate of the (real, OS/hardware) mouse cursor, in
     * window-client pixels, synced fresh each frame from the OS, NOT derived by
     * accumulating get_delta_x() over time (those are independent, raw, unprocessed
     * per-frame deltas straight from the device, this is the actual,
     * OS-ballistics-applied, screen-edge-clamped cursor position). May lag behind
     * by a frame's worth of latency, same as any other polled state.
     */
    int get_x() const { return _x; }

    /**
     * Returns the y coordinate of the (real, OS/hardware) mouse cursor, in
     * window-client pixels, synced fresh each frame from the OS, NOT derived by
     * accumulating get_delta_y() over time (those are independent, raw, unprocessed
     * per-frame deltas straight from the device, this is the actual,
     * OS-ballistics-applied, screen-edge-clamped cursor position). May lag behind
     * by a frame's worth of latency, same as any other polled state.
     */
    int get_y() const { return _y; }

    /**
     * Returns the horizontal mouse movement during this frame, in raw, unprocessed
     * device pixels (no OS pointer-acceleration/sensitivity curve applied,
     * no screen-edge clamping). Suitable for camera-look-style relative control.
     * See get_x() for the actual (OS-synced) cursor position, which this does NOT
     * simply accumulate into.
     */
    int get_delta_x() const { return _delta_x; }

    /**
     * Returns the horizontal mouse movement during this frame, in raw, unprocessed
     * device pixels (no OS pointer-acceleration/sensitivity curve applied,
     * no screen-edge clamping). Suitable for camera-look-style relative control.
     * See get_y() for the actual (OS-synced) cursor position, which this does NOT
     * simply accumulate into.
     */
    int get_delta_y() const { return _delta_y; }

    /**
     * Returns the mouse wheel movement during this frame. One notch/detent of
     * the wheel corresponds to a delta of 1; backends normalize their native
     * units to this.
     */
    int get_wheel_delta() const { return _wheel_delta; }



private:

    // #############################################################################
    // Types
    // #############################################################################

    struct Position
    {
        int x = 0;
        int y = 0;
    };

    struct MouseButtonState
    {
        DigitalState state;

        /** The mouse position at the moment this button was last pressed down. */
        Position position_at_down;

        // Click history for double-click detection, deliberately held back by
        // one frame: a click that completes this frame must not already count
        // as its own "previous click" when double-click is queried this same
        // frame, so the pending click is only committed (becoming visible to
        // queries) at the start of the *next* frame's _apply() call.
        bool has_pending_click = false;
        std::chrono::steady_clock::time_point pending_click_time;
        Position pending_click_position;

        std::chrono::steady_clock::time_point last_click_time;
        Position last_click_position;
    };



    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed values/deltas for this frame.
     */
    void _apply(const std::array<bool, static_cast<size_t>(MouseButton::COUNT)>& button_values, int delta_x, int delta_y, int wheel_delta, std::chrono::steady_clock::time_point now);

    /**
     * Applies a newly observed absolute cursor position (window-client pixels),
     * synced from the OS. Deliberately separate from _apply(): the position isn't
     * derived from that call's (unrelated, raw) deltas.
     */
    void _apply_cursor_position(int x, int y);



    // #############################################################################
    // Helpers
    // #############################################################################

    static int _distance_squared(const Position& a, const Position& b);



    // #############################################################################
    // Attributes
    // #############################################################################

    std::array<MouseButtonState, static_cast<size_t>(MouseButton::COUNT)> _buttons{};

    int _x = 0;
    int _y = 0;
    int _delta_x = 0;
    int _delta_y = 0;
    int _wheel_delta = 0;

    std::chrono::steady_clock::time_point _now;

}; // class Mouse



} // namespace xl7::input

#endif // XL7_INPUT_MOUSE_H
