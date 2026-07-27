#include "GameControllerSystemImpl.h"
#if F77_IS_WINDOWS

#include <windows.h>
#include <xinput.h>

#include <algorithm>
#include <cmath>



namespace xl7::input::impl::xinput {



namespace {

    /**
     * Normalizes a trigger's raw value to [0, 1], applying XInput's documented
     * press threshold as a deadzone.
     */
    float _normalize_trigger(BYTE value, BYTE threshold)
    {
        if (value < threshold)
            return 0.0f;
        return static_cast<float>(value - threshold) / static_cast<float>(255 - threshold);
    }

    /**
     * Normalizes a thumbstick's raw axes, applying XInput's documented radial
     * deadzone. Both axes stay in [-1, 1].
     */
    GameController::RawStickValues _normalize_stick(SHORT raw_x, SHORT raw_y, SHORT deadzone)
    {
        const auto x = static_cast<float>(raw_x);
        const auto y = static_cast<float>(raw_y);
        const float magnitude = std::sqrt(x * x + y * y);

        if (magnitude < static_cast<float>(deadzone))
            return {};

        const float normalized_magnitude = std::min((magnitude - static_cast<float>(deadzone)) / (32767.0f - static_cast<float>(deadzone)), 1.0f);
        const float scale = normalized_magnitude / magnitude;

        return { .x = x * scale, .y = y * scale };
    }

} // namespace



    // #############################################################################
    // GameControllerSystem Implementations
    // #############################################################################

    /**
     * Polls all XInput slots and fills in their raw state.
     */
    void GameControllerSystemImpl::_poll_impl(std::array<RawControllerState, MAX_CONTROLLERS>& out)
    {
        for (unsigned index = 0; index < MAX_CONTROLLERS; ++index)
        {
            RawControllerState& state = out[index];

            XINPUT_STATE xinput_state{};
            if (::XInputGetState(index, &xinput_state) != ERROR_SUCCESS)
            {
                state = {};
                continue;
            }

            state.connected = true;
            state.type = GameControllerType::Gamepad;

            const XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;

            state.button_values[static_cast<size_t>(GameControllerButton::DPadUp)]        = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::DPadDown)]      = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::DPadLeft)]      = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::DPadRight)]     = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::Start)]         = (gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::Back)]          = (gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::LeftThumb)]     = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::RightThumb)]    = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::LeftShoulder)]  = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::RightShoulder)] = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::A)]             = (gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::B)]             = (gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::X)]             = (gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
            state.button_values[static_cast<size_t>(GameControllerButton::Y)]             = (gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

            state.left_trigger_value  = _normalize_trigger(gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
            state.right_trigger_value = _normalize_trigger(gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

            state.left_stick_values  = _normalize_stick(gamepad.sThumbLX, gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            state.right_stick_values = _normalize_stick(gamepad.sThumbRX, gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        } // for each game controller slot
    }



} // namespace xl7::input::impl::xinput

#endif // F77_IS_WINDOWS
