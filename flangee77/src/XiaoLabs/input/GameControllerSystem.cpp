#include "GameControllerSystem.h"

#include "./impl/xinput/GameControllerSystemImpl.h"

#include <CoreLabs/logging.h>

#include <algorithm>
#include <chrono>
#include <cmath>



namespace xl7::input {



namespace {

    /**
     * Sums two stick readings and clamps the result's magnitude back to [0, 1],
     * preserving direction (same technique used to normalize a single raw
     * XInput stick reading against its deadzone).
     */
    GameController::RawStickValues _sum_sticks(const GameController::RawStickValues& a, const GameController::RawStickValues& b)
    {
        GameController::RawStickValues sum{.x = a.x + b.x, .y = a.y + b.y};

        const float magnitude = std::sqrt(sum.x * sum.x + sum.y * sum.y);
        if (magnitude > 1.0f)
        {
            sum.x /= magnitude;
            sum.y /= magnitude;
        }

        return sum;
    }

} // namespace



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    GameControllerSystem::GameControllerSystem()
        : _aggregated_controller(std::make_unique<GameController>())
    {
        for (auto& controller : _controllers)
            controller = std::make_unique<GameController>();
    }



    // #############################################################################
    // Factory Function
    // #############################################################################

    GameControllerSystem* GameControllerSystem::factory_func()
    {
        return backend_registry().resolve(config_provider().get_config().input.game_controller_driver_name);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Polls all backend game controller slots and refreshes state, including
     * the aggregate.
     */
    void GameControllerSystem::update()
    {
        const auto now = std::chrono::steady_clock::now();

        std::array<RawControllerState, MAX_CONTROLLERS> raw{};
        _poll_impl(raw);

        bool aggregated_connected = false;
        std::array<bool, static_cast<size_t>(GameControllerButton::COUNT)> aggregated_button_values{};
        float aggregated_left_trigger_value = 0.0f;
        float aggregated_right_trigger_value = 0.0f;
        GameController::RawStickValues aggregated_left_stick_values;
        GameController::RawStickValues aggregated_right_stick_values;

        for (size_t i = 0; i < MAX_CONTROLLERS; ++i)
        {
            const RawControllerState& state = raw[i];

            GameController::Attorney::apply(*_controllers[i], state.connected, state.type, state.button_values, state.left_trigger_value, state.right_trigger_value, state.left_stick_values, state.right_stick_values, now);

            aggregated_connected = aggregated_connected || state.connected;
            for (size_t b = 0; b < aggregated_button_values.size(); ++b)
                aggregated_button_values[b] = aggregated_button_values[b] || state.button_values[b];
            aggregated_left_trigger_value = std::min(aggregated_left_trigger_value + state.left_trigger_value, 1.0f);
            aggregated_right_trigger_value = std::min(aggregated_right_trigger_value + state.right_trigger_value, 1.0f);
            aggregated_left_stick_values = _sum_sticks(aggregated_left_stick_values, state.left_stick_values);
            aggregated_right_stick_values = _sum_sticks(aggregated_right_stick_values, state.right_stick_values);
        }

        // The aggregate's "type" isn't well-defined once multiple, possibly
        // different, controllers are merged, so it's always reported as Unknown.
        GameController::Attorney::apply(*_aggregated_controller, aggregated_connected, GameControllerType::Unknown, aggregated_button_values, aggregated_left_trigger_value, aggregated_right_trigger_value, aggregated_left_stick_values, aggregated_right_stick_values, now);
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool GameControllerSystem::_init()
    {
        LOG_SUCCESS(u8"The game controller input component based on " + cl7::u8string{get_driver_name()} + u8" has been successfully initialized.");
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool GameControllerSystem::_shutdown()
    {
        LOG_SUCCESS(u8"The game controller input component based on " + cl7::u8string{get_driver_name()} + u8" has been shut down successfully.");
        return true;
    }



} // namespace xl7::input
