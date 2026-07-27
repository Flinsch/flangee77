#include "Keyboard.h"



namespace xl7::input {



    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed values/text for this frame.
     */
    void Keyboard::_apply(const std::array<bool, static_cast<size_t>(Key::COUNT)>& key_values, const cl7::u8string& text_input_delta, std::chrono::steady_clock::time_point now)
    {
        for (size_t i = 0; i < key_values.size(); ++i)
            _keys[i].update(key_values[i], now);

        _text_input = text_input_delta;
        _now = now;
    }



} // namespace xl7::input
