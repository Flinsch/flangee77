#ifndef XL7_GRAPHICS_STATES_MINMAGFILTERTYPE_H
#define XL7_GRAPHICS_STATES_MINMAGFILTERTYPE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** The filtering method to use when sampling for minification and magnification. */
enum struct MinMagFilterType
{
    /** Use point sampling. */
    Point = 1,
    /** Use linear interpolation. */
    Linear,
    /** Use anisotropic interpolation. */
    Anisotropic,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_MINMAGFILTERTYPE_H
