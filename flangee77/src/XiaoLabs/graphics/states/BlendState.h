#pragma once
#ifndef XL7_GRAPHICS_STATES_BLENDSTATE_H
#define XL7_GRAPHICS_STATES_BLENDSTATE_H
#include "./AbstractState.h"

#include "../ChannelFlags.h"



namespace xl7 {
namespace graphics {
namespace states {



class BlendState
    : public AbstractState
{

public:
    enum class BlendFactor
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

    enum class BlendOperation
    {
        Add = 1,
        Subtract,
        RevSubtract,
        Min,
        Max,
    };

public:
    struct Desc
    {
        /** Enables (true) or disables (false) blending in general. */
        bool is_blending_enabled        = false;
        /** . */
        BlendFactor src_color_factor    = BlendFactor::One;
        /** . */
        BlendFactor dest_color_factor   = BlendFactor::Zero;
        /** . */
        BlendOperation color_operation  = BlendOperation::Add;
        /** . */
        BlendFactor src_alpha_factor    = BlendFactor::One;
        /** . */
        BlendFactor dest_alpha_factor   = BlendFactor::Zero;
        /** . */
        BlendOperation alpha_operation  = BlendOperation::Add;
        /** . */
        ChannelFlags channel_write_flags= ChannelFlags::All;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    BlendState(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~BlendState() = default;

private:
    /** Default constructor. */
    BlendState() = delete;
    /** Copy constructor. */
    BlendState(const BlendState&) = delete;
    /** Copy assignment operator. */
    BlendState& operator = (const BlendState&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the sampler state.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the sampler state.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("blend state"); }

}; // class BlendState



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_BLENDSTATE_H
