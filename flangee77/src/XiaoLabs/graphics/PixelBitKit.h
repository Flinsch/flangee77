#pragma once
#ifndef XL7_GRAPHICS_PIXELBITKIT_H
#define XL7_GRAPHICS_PIXELBITKIT_H

#include "./PixelFormat.h"
#include "./ChannelOrder.h"



namespace xl7 {
namespace graphics {



struct PixelBitKit
{

    enum class DataType
    {
        UNKNOWN,

        UNORM,
        SNORM,
        UINT,
        SINT,
        FLOAT,
    };

    struct Channel
    {
        /** The bit depth of the channel (the size of the channel, in bits). */
        unsigned depth;
        /** The offset/shift of the channel, in bits. */
        unsigned offset;
        /** The mask of the channel within a pixel (bit-depth 1s shifted by offset bits). */
        unsigned long long mask;
    };



    /** The pixel format. */
    PixelFormat pixel_format;
    /** The channel order. */
    ChannelOrder channel_order;

    /** The number of channels in a pixel. */
    unsigned channel_count;

    /** */
    union
    {
        struct
        {
            Channel r, g, b, a;
        };
        Channel channels[4];
    };

    /** */
    DataType data_type;

    /** The size of one pixel, in bytes. */
    unsigned stride;

    ///** The number of bytes from the beginning of one line of pixels to the beginning of the next. */
    //unsigned pitch;



    PixelBitKit(PixelFormat pixel_format, ChannelOrder channel_order);

}; // struct PixelBitKit



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIXELBITKIT_H
