#include "AnalogState.h"



namespace xl7::input {



    /**
     * Applies the newly observed value for this frame: latches the current value as
     * the previous one and applies the new value.
     */
    void AnalogState::update(float new_value)
    {
        previous = current;
        current = new_value;
    }



} // namespace xl7::input
