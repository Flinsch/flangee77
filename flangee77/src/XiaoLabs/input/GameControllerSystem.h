#ifndef XL7_INPUT_GAMECONTROLLERSYSTEM_H
#define XL7_INPUT_GAMECONTROLLERSYSTEM_H
#include "../DriverBasedComponent.h"

#include "./GameController.h"

#include <array>
#include <memory>



namespace xl7::input {



class GameControllerSystem
    : public DriverBasedComponent<GameControllerSystem>
{
    friend class cl7::creational::Singleton<GameControllerSystem>;

public:
    /**
     * The number of simultaneously supported game controller slots. Matches
     * XInput's current limit; may grow once other backends support more.
     */
    static constexpr unsigned MAX_CONTROLLERS = 4;

    /**
     * Raw, backend-produced state for a single controller slot for a given
     * frame. Backends fill this in; GameControllerSystem::update() applies it
     * to the corresponding GameController (and folds it into the aggregate).
     */
    struct RawControllerState
    {
        bool connected = false;
        GameControllerType type = GameControllerType::Unknown;
        std::array<bool, static_cast<size_t>(GameControllerButton::COUNT)> button_values{};
        float left_trigger_value = 0.0f;
        float right_trigger_value = 0.0f;
        GameController::RawStickValues left_stick_values{};
        GameController::RawStickValues right_stick_values{};
    };



    GameControllerSystem(const GameControllerSystem&) = delete;
    GameControllerSystem& operator=(const GameControllerSystem&) = delete;
    GameControllerSystem(GameControllerSystem&&) = delete;
    GameControllerSystem& operator=(GameControllerSystem&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the game controller in the specified slot (0-based, up to
     * MAX_CONTROLLERS - 1).
     */
    GameController& get_game_controller(unsigned index) const { assert(index < MAX_CONTROLLERS); return *_controllers[index]; }

    /**
     * Returns the aggregate of all connected game controllers: a button/key is
     * "down" if at least one controller reports it down; triggers and sticks
     * are summed (clamped back into their valid range).
     */
    GameController& aggregated_game_controller() const { return *_aggregated_controller; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Polls all backend game controller slots and refreshes state, including
     * the aggregate. Must be called once per frame.
     */
    void update();



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    GameControllerSystem();
    ~GameControllerSystem() override = default;



    // #############################################################################
    // Factory Function
    // #############################################################################

    static GameControllerSystem* factory_func();



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Polls all controller slots and fills in their raw state.
     */
    virtual void _poll_impl(std::array<RawControllerState, MAX_CONTROLLERS>& out) = 0;



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool _init() final;

    /**
     * De-initializes the component.
     */
    bool _shutdown() final;



    // #############################################################################
    // Attributes
    // #############################################################################

    std::array<std::unique_ptr<GameController>, MAX_CONTROLLERS> _controllers;
    std::unique_ptr<GameController> _aggregated_controller;

}; // class GameControllerSystem



} // namespace xl7::input

#endif // XL7_INPUT_GAMECONTROLLERSYSTEM_H
