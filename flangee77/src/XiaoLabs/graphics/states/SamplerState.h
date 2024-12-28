#pragma once
#ifndef XL7_GRAPHICS_STATES_SAMPLERSTATE_H
#define XL7_GRAPHICS_STATES_SAMPLERSTATE_H
#include "./AbstractState.h"

#include "../Color.h"

#include <cfloat>



namespace xl7::graphics::states {



class SamplerState // NOLINT(*-virtual-class-destructor)
    : public AbstractState
{

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

    struct Desc
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
        float min_lod   = -FLT_MAX;
        /** The upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to min_lod. To have no upper limit, set this to a large value such as FLT_MAX. */
        float max_lod   = +FLT_MAX;

        /** The maximum anisotropy to use (if applicable). Values range from 1 (default) to the maximum supported anisotropy specified in the capabilities of the rendering device. */
        unsigned max_anisotropy = 1;
    };



    SamplerState() = delete;

    SamplerState(const SamplerState&) = delete;
    SamplerState& operator=(const SamplerState&) = delete;
    SamplerState(SamplerState&&) = delete;
    SamplerState& operator=(SamplerState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"sampler state"; }

    /**
     * Returns the descriptor of the sampler state.
     */
    const Desc& get_desc() const { return _desc; }



protected:
    SamplerState(const CreateParams<Desc>& params)
        : AbstractState(params)
        , _desc(params.desc)
    {
    }

    ~SamplerState() override = default;



private:
    /**
     * The descriptor of the sampler state.
     */
    const Desc _desc;

}; // class SamplerState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_SAMPLERSTATE_H
