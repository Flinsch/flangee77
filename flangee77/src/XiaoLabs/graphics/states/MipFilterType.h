#ifndef XL7_GRAPHICS_STATES_MIPFILTERTYPE_H
#define XL7_GRAPHICS_STATES_MIPFILTERTYPE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** The filtering method to use when sampling for mip-level sampling. */
enum struct MipFilterType
{
    /** Disables mipmapping. */
    None,
    /** Use point sampling between mip levels. */
    Point = 1,
    /** Use linear interpolation between mip levels. */
    Linear,
    /** Use anisotropic filtering for mip-level sampling (if supported). */
    Anisotropic,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_MIPFILTERTYPE_H
