#ifndef XL7_INPUT_DIGITALSTATE_H
#define XL7_INPUT_DIGITALSTATE_H

#include <CoreLabs/root.h>

#include <chrono>



namespace xl7::input {



/**
 * The minimal state needed to derive down/pressed/released/tapped queries for a
 * single key/button: its current value, its value as of the previous frame (for
 * edge detection), and the point in time it was last pressed down (for the tap
 * timing check).
 */
struct DigitalState
{
    /**
     * The default maximum down-to-up duration for a down/up cycle to still count as
     * a "tap" (see is_tapped()). Callers may override this per query.
     */
    static constexpr auto DEFAULT_TAP_TIME_WINDOW = std::chrono::milliseconds(200);



    bool current = false;
    bool previous = false;
    std::chrono::steady_clock::time_point down_since;



    /**
     * Applies the newly observed value for this frame: latches the current value as
     * the previous one, applies the new value, and (only on a rising edge) records
     * the point in time this happened.
     */
    void update(bool new_value, std::chrono::steady_clock::time_point now);

    /**
     * Returns whether the button/key is currently held down.
     */
    bool is_down() const { return current; }

    /**
     * Returns whether the button/key was pressed down during this frame (down now,
     * up before).
     */
    bool is_pressed() const { return current && !previous; }

    /**
     * Returns whether the button/key was released during this frame (up now, down
     * before).
     */
    bool is_released() const { return !current && previous; }

    /**
     * Returns whether the button/key was released during this frame, and the down/up
     * cycle it completed fit within the specified time window (i.e., a "tap").
     */
    bool is_tapped(std::chrono::steady_clock::time_point now, std::chrono::milliseconds time_window = DEFAULT_TAP_TIME_WINDOW) const;

}; // struct DigitalState


} // namespace xl7::input

#endif // XL7_INPUT_DIGITALSTATE_H
