#pragma once
#ifndef XL7_GRAPHICS_CHANNELFLAGS_H
#define XL7_GRAPHICS_CHANNELFLAGS_H

#include <CoreLabs/root.h>



namespace xl7 {
namespace graphics {



enum class ChannelFlags
{
    Red     = 0x1,
    Green   = 0x2,
    Blue    = 0x4,
    Alpha   = 0x8,

    RGB     = 0x7,
    All     = 0xf,
};



inline ChannelFlags operator | (ChannelFlags a, ChannelFlags b)
{
    return static_cast<ChannelFlags>( static_cast<unsigned>( a ) | static_cast<unsigned>( b ) );
}

inline ChannelFlags operator & (ChannelFlags a, ChannelFlags b)
{
    return static_cast<ChannelFlags>( static_cast<unsigned>( a ) & static_cast<unsigned>( b ) );
}



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_CHANNELFLAGS_H
