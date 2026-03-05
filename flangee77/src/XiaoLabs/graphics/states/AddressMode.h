#ifndef XL7_GRAPHICS_STATES_ADDRESSMODE_H
#define XL7_GRAPHICS_STATES_ADDRESSMODE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Identifies the technique for resolving texture coordinates that lie outside the boundaries of a texture. */
enum struct AddressMode
{
    /** Tile the texture at every integer junction. For example, for values between 0 and 3, the texture is repeated three times. */
    Wrap = 1,
    /** Flip the texture at every integer junction. For values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on. */
    Mirror,
    /** Texture coordinates outside the range [0;1] are set to the texture color at 0 or 1, respectively. */
    Clamp,
    /** Texture coordinates outside the range [0;1] are set to the specified border color. */
    Border,
    /** Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value. */
    MirrorOnce,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_ADDRESSMODE_H
