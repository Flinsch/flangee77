#ifndef XL7_GRAPHICS_CHANNELORDER_H
#define XL7_GRAPHICS_CHANNELORDER_H

#include <CoreLabs/string.h>



namespace xl7::graphics {



/**
 * Describes the logical ordering of color channels within a pixel or texel. The
 * channel order affects how component data in memory is mapped to semantic color
 * channels (R, G, B, A).
 *
 * PixelFormat and ChannelOrder together fully describe the in-memory layout.
 */
enum struct ChannelOrder
{
    /** RGBA: Most common modern graphics API convention. */
    RGBA,
    /** ARGB: Common in legacy APIs and some image file formats. */
    ARGB,
    /** ABGR: Used by certain platforms and CPU-side pixel buffers. */
    ABGR,
    /** BGRA: Common Windows-native format and Direct3D interoperability. Corresponds to D3DCOLOR in Direct3D 9. */
    BGRA,
};



} // namespace xl7::graphics



namespace cl7 {
    cl7::u8string to_string(xl7::graphics::ChannelOrder channel_order);
}



#endif // XL7_GRAPHICS_CHANNELORDER_H
