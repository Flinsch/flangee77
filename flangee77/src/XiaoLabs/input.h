#ifndef XL7_INPUT_H
#define XL7_INPUT_H

#include "./input/KeyboardMouseSystem.h"
#include "./input/GameControllerSystem.h"



namespace xl7::input {



    inline bool has_available_keyboard_mouse_backend() { return KeyboardMouseSystem::has_available_backend(); }
    inline KeyboardMouseSystem& keyboard_mouse_system() { return KeyboardMouseSystem::instance(); }

    inline bool has_available_game_controller_backend() { return GameControllerSystem::has_available_backend(); }
    inline GameControllerSystem& game_controller_system() { return GameControllerSystem::instance(); }

    /**
     * Convenience accessor for the aggregate of all connected keyboards, for
     * callers that don't care which actual keyboard did it.
     */
    inline Keyboard& keyboard() { return keyboard_mouse_system().aggregated_keyboard(); }

    /**
     * Convenience accessor for the aggregate of all connected mice, for callers
     * that don't care which actual mouse did it.
     */
    inline Mouse& mouse() { return keyboard_mouse_system().aggregated_mouse(); }

    /**
     * Convenience accessor for the aggregate of all connected game controllers,
     * for callers that don't care which actual game controller did it.
     */
    inline GameController& game_controller() { return game_controller_system().aggregated_game_controller(); }



} // namespace xl7::input

#endif // XL7_INPUT_H
