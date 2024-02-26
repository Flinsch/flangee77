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
    /** The size of one pixel, in bytes. */
    unsigned stride;

    /** */
    union
    {
        struct
        {
            Channel r, g, b, a;
        };
        Channel channels[4];
    };

    /** The data type of each channel. */
    DataType data_type;

    ///** The number of bytes from the beginning of one line of pixels to the beginning of the next. */
    //unsigned pitch;



    PixelBitKit(PixelFormat pixel_format, ChannelOrder channel_order);



    /** Returns the channel count of the specified pixel format. */
    static unsigned determine_channel_count(PixelFormat pixel_format);

    /** Returns the number of bytes of one pixel of the specified format. */
    static unsigned determine_stride(PixelFormat pixel_format);

    /** Returns the data type of the specified pixel format. */
    static DataType determine_data_type(PixelFormat pixel_format);

}; // struct PixelBitKit



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIXELBITKIT_H
