#ifndef XL7_GRAPHICS_STATES_SAMPLERSTATEDESC_H
#define XL7_GRAPHICS_STATES_SAMPLERSTATEDESC_H

#include "./MinMagFilterType.h"
#include "./MipFilterType.h"
#include "./AddressMode.h"

#include "../Color.h"

#include <limits>



namespace xl7::graphics::states {



struct SamplerStateDesc
{
    /** The filtering method to use when sampling for minification. */
    MinMagFilterType min_filter_type    = MinMagFilterType::Linear;
    /** The filtering method to use when sampling for magnification. */
    MinMagFilterType mag_filter_type    = MinMagFilterType::Linear;
    /** The filtering method to use when sampling for mip-level sampling. */
    MipFilterType mip_filter_type       = MipFilterType::Linear;

    /** The method to use for resolving a u texture coordinate that is outside the 0 to 1 range. */
    AddressMode address_u   = AddressMode::Clamp;
    /** The method to use for resolving a v texture coordinate that is outside the 0 to 1 range. */
    AddressMode address_v   = AddressMode::Clamp;
    /** The method to use for resolving a w texture coordinate that is outside the 0 to 1 range. */
    AddressMode address_w   = AddressMode::Clamp;

    /** The border color to use if "Border" is specified for address_u, address_v, or address_w. */
    Color border_color  = Color::ZERO;

    /** The offset from the calculated mipmap level (default: 0). For example, if a texture should actually be sampled at mipmap level 3 and mip_lod_bias is 2, the texture will be sampled at mipmap level 5. */
    float lod_bias  = 0.0f;
    /** The lower end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. */
    float min_lod   = std::numeric_limits<float>::lowest();
    /** The upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to min_lod. To have no upper limit, set this to a large value such as FLT_MAX. */
    float max_lod   = std::numeric_limits<float>::max();

    /** The maximum anisotropy to use (if applicable). Values range from 1 (default) to the maximum supported anisotropy specified in the capabilities of the rendering device. */
    unsigned max_anisotropy = 1;
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_SAMPLERSTATEDESC_H
