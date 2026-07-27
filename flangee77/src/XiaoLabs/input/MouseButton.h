#ifndef XL7_INPUT_MOUSEBUTTON_H
#define XL7_INPUT_MOUSEBUTTON_H

#include <CoreLabs/root.h>



namespace xl7::input {



/**
 * Identifies a mouse button. X1/X2 are the (optional) extra side buttons found
 * on many mice, typically bound to "back"/"forward" by convention.
 */
enum struct MouseButton
{
    Left,
    Right,
    Middle,
    X1,
    X2,

    /** Not a real button: sentinel for sizing lookup tables. Keep it last. */
    COUNT,
};



} // namespace xl7::input

#endif // XL7_INPUT_MOUSEBUTTON_H
