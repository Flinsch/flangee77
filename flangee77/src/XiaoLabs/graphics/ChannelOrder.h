#pragma once
#ifndef XL7_GRAPHICS_CHANNELORDER_H
#define XL7_GRAPHICS_CHANNELORDER_H

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {



enum class ChannelOrder
{
    RGBA,
    ARGB,
    ABGR,
    BGRA, // Corresponds to D3DCOLOR of Direct3D 9.
};



} // namespace graphics
} // namespace xl7



namespace cl7 {
    cl7::string to_string(xl7::graphics::ChannelOrder channel_order);
}



#endif // XL7_GRAPHICS_CHANNELORDER_H
