#ifndef XL7_INPUT_GAMECONTROLLER_H
#define XL7_INPUT_GAMECONTROLLER_H

#include "./AnalogState.h"
#include "./DigitalState.h"
#include "./GameControllerButton.h"
#include "./GameControllerType.h"

#include <array>
#include <chrono>



namespace xl7::input {



/**
 * Represents a single game controller's state (or the aggregate of all
 * connected game controllers, which is fed the same way, just with OR'd
 * button values and summed trigger/stick values instead of a single physical
 * device's raw signals).
 */
class GameController
{

public:
    /**
     * A single, backend-supplied thumbstick reading for one frame: raw x/y axis
     * values, already deadzone-corrected by the backend, each in [-1, 1]. This is
     * the "new value" fed in for a frame.
     */
    struct RawStickValues
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    /**
     * A 2D thumbstick's tracked state: one AnalogState per axis, each holding this
     * frame's and the previous frame's deadzone-corrected value, in [-1, 1].
     */
    struct StickState
    {
        AnalogState x{};
        AnalogState y{};

        void update(RawStickValues new_values)
        {
            x.update(new_values.x);
            y.update(new_values.y);
        }
    };

    /**
     * Grants GameControllerSystem exclusive access to feed new state into a
     * GameController instance, keeping that (backend-facing) capability out
     * of the public query API.
     */
    class Attorney
    {
        static void apply(GameController& controller, bool connected, GameControllerType type, const std::array<bool, static_cast<size_t>(GameControllerButton::COUNT)>& button_values, float left_trigger_value, float right_trigger_value, RawStickValues left_stick_values, RawStickValues right_stick_values, std::chrono::steady_clock::time_point now) { controller._apply(connected, type, button_values, left_trigger_value, right_trigger_value, left_stick_values, right_stick_values, now); }
        friend class GameControllerSystem;
    };



    GameController() = default;

    GameController(const GameController&) = delete;
    GameController& operator=(const GameController&) = delete;
    GameController(GameController&&) = delete;
    GameController& operator=(GameController&&) = delete;

    ~GameController() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns whether this game controller is currently connected.
     */
    bool is_connected() const { return _connected; }

    /**
     * Returns the (informational) type of this game controller.
     */
    GameControllerType get_type() const { return _type; }

    /**
     * Returns the state of the specified button.
     */
    const DigitalState& get_button(GameControllerButton button) const { return _buttons[static_cast<size_t>(button)]; }

    /**
     * Returns whether the specified button is currently held down.
     */
    bool is_button_down(GameControllerButton button) const { return _buttons[static_cast<size_t>(button)].is_down(); }

    /**
     * Returns whether the specified button was pressed down during this frame.
     */
    bool is_button_pressed(GameControllerButton button) const { return _buttons[static_cast<size_t>(button)].is_pressed(); }

    /**
     * Returns whether the specified button was released during this frame.
     */
    bool is_button_released(GameControllerButton button) const { return _buttons[static_cast<size_t>(button)].is_released(); }

    /**
     * Returns whether the specified button was released during this frame, and the
     * down/up cycle it completed fit within the specified time window.
     */
    bool is_button_tapped(GameControllerButton button, std::chrono::milliseconds time_window = DigitalState::DEFAULT_TAP_TIME_WINDOW) const { return _buttons[static_cast<size_t>(button)].is_tapped(_now, time_window); }

    /**
     * Returns the state of the left trigger.
     */
    const AnalogState& get_left_trigger() const { return _left_trigger; }

    /**
     * Returns the state of the right trigger.
     */
    const AnalogState& get_right_trigger() const { return _right_trigger; }

    /**
     * Returns the state of the left thumbstick.
     */
    const StickState& get_left_stick() const { return _left_stick; }

    /**
     * Returns the state of the right thumbstick.
     */
    const StickState& get_right_stick() const { return _right_stick; }



private:

    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed state for this frame.
     */
    void _apply(bool connected, GameControllerType type, const std::array<bool, static_cast<size_t>(GameControllerButton::COUNT)>& button_values, float left_trigger_value, float right_trigger_value, RawStickValues left_stick_values, RawStickValues right_stick_values, std::chrono::steady_clock::time_point now);



    // #############################################################################
    // Attributes
    // #############################################################################

    bool _connected = false;
    GameControllerType _type = GameControllerType::Unknown;

    std::array<DigitalState, static_cast<size_t>(GameControllerButton::COUNT)> _buttons{};

    AnalogState _left_trigger{};
    AnalogState _right_trigger{};

    StickState _left_stick;
    StickState _right_stick;

    std::chrono::steady_clock::time_point _now;

}; // class GameController



} // namespace xl7::input

#endif // XL7_INPUT_GAMECONTROLLER_H
