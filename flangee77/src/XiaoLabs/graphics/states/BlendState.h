#ifndef XL7_GRAPHICS_STATES_BLENDSTATE_H
#define XL7_GRAPHICS_STATES_BLENDSTATE_H
#include "./AbstractState.h"

#include "../ChannelFlags.h"



namespace xl7::graphics::states {



class BlendState
    : public resources::detail::ResourceBase<BlendState, AbstractState>
{

public:
    enum struct BlendFactor
    {
        Zero = 1,
        One,
        SrcColor,
        InvSrcColor,
        SrcAlpha,
        InvSrcAlpha,
        DestAlpha,
        InvDestAlpha,
        DestColor,
        InvDestColor,
        SrcAlphaSat,
        BlendFactor = 14, // Force compatibility with Direct3D 9 and 11
        InvBlendFactor,
    };

    enum struct BlendOperation
    {
        Add = 1,
        Subtract,
        RevSubtract,
        Min,
        Max,
    };

    struct Desc
    {
        /** Enables (true) or disables (false) blending in general. */
        bool is_blending_enabled            = false;
        /** . */
        BlendFactor src_color_factor        = BlendFactor::One;
        /** . */
        BlendFactor dest_color_factor       = BlendFactor::Zero;
        /** . */
        BlendOperation color_operation      = BlendOperation::Add;
        /** . */
        BlendFactor src_alpha_factor        = BlendFactor::One;
        /** . */
        BlendFactor dest_alpha_factor       = BlendFactor::Zero;
        /** . */
        BlendOperation alpha_operation      = BlendOperation::Add;
        /** . */
        ChannelFlags channel_write_flags    = ChannelFlags::All;
    };



    BlendState() = delete;

    BlendState(const BlendState&) = delete;
    BlendState& operator=(const BlendState&) = delete;
    BlendState(BlendState&&) = delete;
    BlendState& operator=(BlendState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"blend state"; }

    /**
     * Returns the descriptor of the blend state.
     */
    const Desc& get_desc() const { return _desc; }



protected:

    explicit BlendState(const CreateParams<Desc>& params)
        : ResourceBase(params)
        , _desc(params.desc)
    {
    }

    ~BlendState() override = default;



private:
    /**
     * The descriptor of the blend state.
     */
    const Desc _desc;

}; // class BlendState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDSTATE_H
