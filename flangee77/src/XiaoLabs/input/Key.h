#ifndef XL7_INPUT_KEY_H
#define XL7_INPUT_KEY_H

#include <CoreLabs/root.h>



namespace xl7::input {



/**
 * Identifies a physical/logical keyboard key in a backend-agnostic way. Values
 * are not tied to any platform's native key codes; each backend translates its
 * own codes to/from this enumeration.
 */
enum struct Key
{
    Unknown,

    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Digits (top row)
    Digit0, Digit1, Digit2, Digit3, Digit4,
    Digit5, Digit6, Digit7, Digit8, Digit9,

    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    // Editing/whitespace
    Space,
    Enter,
    Escape,
    Tab,
    Backspace,
    Delete,
    Insert,

    // Navigation
    Left,
    Right,
    Up,
    Down,
    Home,
    End,
    PageUp,
    PageDown,

    // Modifiers
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,

    /** Not a real key: sentinel for sizing lookup tables. Keep it last. */
    COUNT,
};



} // namespace xl7::input

#endif // XL7_INPUT_KEY_H
