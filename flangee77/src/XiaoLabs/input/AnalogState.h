#ifndef XL7_INPUT_ANALOGSTATE_H
#define XL7_INPUT_ANALOGSTATE_H

#include <CoreLabs/root.h>



namespace xl7::input {



/**
 * The minimal state needed to derive current/previous-value queries for a single
 * continuous (analog) input signal, such as a trigger's position or one axis of a
 * thumbstick: its current value and its value as of the previous frame (e.g., for
 * computing a frame-to-frame delta).
 */
struct AnalogState
{
    float current = 0.0f;
    float previous = 0.0f;



    /**
     * Applies the newly observed value for this frame: latches the current value as
     * the previous one and applies the new value.
     */
    void update(float new_value);

}; // struct AnalogState


} // namespace xl7::input

#endif // XL7_INPUT_ANALOGSTATE_H
