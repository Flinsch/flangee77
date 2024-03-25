#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
#include "./AbstractStage.h"

#include "../shaders/Shader.h"
#include "../textures/Texture.h"

#include "../Color.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class AbstractShaderStage
    : public AbstractStage
{

public:
    /** The maximum number of texture/sampler slots (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_TEXTURE_SAMPLER_SLOTS = 16;

public:
    /** The filtering method to use when sampling for minification and magnification. */
    enum class MinMagFilterType
    {
        /** Use point sampling. */
        Point = 1,
        /** Use linear interpolation. */
        Linear,
        /** Use anisotropic interpolation. */
        Anisotropic,
    };

    /** The filtering method to use when sampling for mip-level sampling. */
    enum class MipFilterType
    {
        /** Disables mipmapping. */
        None,
        /** Use point sampling. */
        Point = 1,
        /** Use linear interpolation. */
        Linear,
        /** Use anisotropic interpolation. */
        Anisotropic,
    };

    /** Identifies the technique for resolving texture coordinates that are outside of the boundaries of a texture. */
    enum class AddressMode
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

    /** . */
    struct SamplerDesc
    {
        /** The filtering method to use when sampling for minification. */
        MinMagFilterType min_filter_type;
        /** The filtering method to use when sampling for magnification. */
        MinMagFilterType mag_filter_type;
        /** The filtering method to use when sampling for mip-level sampling. */
        MipFilterType mip_filter_type;

        /** The method to use for resolving a u texture coordinate that is outside the 0 to 1 range. */
        AddressMode address_u;
        /** The method to use for resolving a v texture coordinate that is outside the 0 to 1 range. */
        AddressMode address_v;
        /** The method to use for resolving a w texture coordinate that is outside the 0 to 1 range. */
        AddressMode address_w;

        /** The border color to use if "Border" is specified for address_u, address_v, or address_w. */
        Color border_color;

        /** The offset from the calculated mipmap level (default: 0). For example, if a texture should actually be sampled at mipmap level 3 and mip_lod_bias is 2, the texture will be sampled at mipmap level 5. */
        float mip_lod_bias;
        /** The lower end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. */
        float min_lod;
        /** The upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to min_lod. To have no upper limit, set this to a large value such as FLT_MAX. */
        float max_lod;

        /** The maximum anisotropy to use (if applicable). Values range from 1 (default) to the maximum supported anisotropy specified in the capabilities of the rendering device. */
        unsigned max_anisotropy;
    };

private:
    static constexpr unsigned DIRTY_FLAG_SHADER     = 0x1;
    static constexpr unsigned DIRTY_FLAG_TEXTURE    = 0x2;



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( shader_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_SHADER );
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY( texture_id, MAX_TEXTURE_SAMPLER_SLOTS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_TEXTURE );

}; // class AbstractShaderStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_ABSTRACTSHADERSTAGE_H
