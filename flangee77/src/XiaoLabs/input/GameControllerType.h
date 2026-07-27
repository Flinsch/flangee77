#ifndef XL7_INPUT_GAMECONTROLLERTYPE_H
#define XL7_INPUT_GAMECONTROLLERTYPE_H

#include <CoreLabs/root.h>



namespace xl7::input {



/**
 * Informational classification of a connected game controller. Rarely needed
 * (most games treat all game controllers the same), but some backends can
 * report it, so it's exposed rather than discarded.
 */
enum struct GameControllerType
{
    Unknown,
    Gamepad,
    Joystick,
    Wheel,
};



} // namespace xl7::input

#endif // XL7_INPUT_GAMECONTROLLERTYPE_H
