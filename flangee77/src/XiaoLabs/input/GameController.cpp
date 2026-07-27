#include "GameController.h"



namespace xl7::input {



    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed state for this frame.
     */
    void GameController::_apply(bool connected, GameControllerType type, const std::array<bool, static_cast<size_t>(GameControllerButton::COUNT)>& buttons, float left_trigger_value, float right_trigger_value, RawStickValues left_stick_values, RawStickValues right_stick_values, std::chrono::steady_clock::time_point now)
    {
        _connected = connected;
        _type = type;

        for (size_t i = 0; i < buttons.size(); ++i)
            _buttons[i].update(buttons[i], now);

        _left_trigger.update(left_trigger_value);
        _right_trigger.update(right_trigger_value);
        _left_stick.update(left_stick_values);
        _right_stick.update(right_stick_values);

        _now = now;
    }



} // namespace xl7::input
