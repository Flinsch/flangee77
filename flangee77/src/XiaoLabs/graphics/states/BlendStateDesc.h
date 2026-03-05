#ifndef XL7_GRAPHICS_STATES_BLENDSTATEDESC_H
#define XL7_GRAPHICS_STATES_BLENDSTATEDESC_H

#include "./BlendFactor.h"
#include "./BlendOperation.h"

#include "../ChannelFlags.h"



namespace xl7::graphics::states {



struct BlendStateDesc
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



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDSTATEDESC_H
