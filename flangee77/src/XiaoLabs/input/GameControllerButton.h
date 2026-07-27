#ifndef XL7_INPUT_GAMECONTROLLERBUTTON_H
#define XL7_INPUT_GAMECONTROLLERBUTTON_H

#include <CoreLabs/root.h>



namespace xl7::input {



/**
 * Identifies a game controller button in a backend-agnostic way. Mirrors the
 * XInput button set, which is generic enough to also cover most other
 * gamepad-like controllers (A/B/X/Y correspond to the bottom face-button
 * layout of a standard Xbox-style pad).
 */
enum struct GameControllerButton
{
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
    Start,
    Back,
    LeftThumb,
    RightThumb,
    LeftShoulder,
    RightShoulder,
    A,
    B,
    X,
    Y,

    /** Not a real button: sentinel for sizing lookup tables. Keep it last. */
    COUNT,
};



} // namespace xl7::input

#endif // XL7_INPUT_GAMECONTROLLERBUTTON_H
