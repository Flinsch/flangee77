#include "DigitalState.h"



namespace xl7::input {



    /**
     * Applies the newly observed value for this frame: latches the current value as
     * the previous one, applies the new value, and (only on a rising edge) records
     * the point in time this happened.
     */
    void DigitalState::update(bool new_value, std::chrono::steady_clock::time_point now)
    {
        previous = current;
        current = new_value;

        if (current && !previous)
            down_since = now;
    }

    /**
     * Returns whether the button/key was released during this frame, and the down/up
     * cycle it completed fit within the specified time window (i.e., a "tap").
     */
    bool DigitalState::is_tapped(std::chrono::steady_clock::time_point now, std::chrono::milliseconds time_window) const
    {
        return is_released() && (now - down_since <= time_window);
    }



} // namespace xl7::input
