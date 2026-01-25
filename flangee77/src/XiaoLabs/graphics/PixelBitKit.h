#ifndef XL7_GRAPHICS_PIXELBITKIT_H
#define XL7_GRAPHICS_PIXELBITKIT_H

#include "./PixelFormat.h"
#include "./ChannelOrder.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_span.h>



namespace xl7::graphics {



struct PixelBitKit
{

    enum struct DataType
    {
        /** Unspecified or invalid data type. */
        UNKNOWN,

        /** Unsigned normalized integer, mapped to [0.0, 1.0]. */
        UNORM,
        /** Signed normalized integer, mapped to [-1.0, +1.0]. */
        SNORM,

        /** Unsigned integer, stored and read as-is. */
        UINT,
        /** Signed integer, stored and read as-is. */
        SINT,

        /** IEEE-754 floating point, stored and read as-is. */
        FLOAT,
    };

    struct Channel
    {
        /** The 0-based index of the channel within a pixel. */
        unsigned index;
        /** The bit depth of the channel (the size of the channel, in bits). This is also the main indicator of whether the channel is involved at all. */
        unsigned depth;
        /** The offset/shift of the channel, in bits. */
        unsigned offset;
        /** The mask of the channel within a pixel (bit-depth 1s shifted by offset bits). */
        uint64_t mask;
        /** The "standalone" mask of the channel (bit-depth 1s shifted by 0 bits). */
        uint64_t mask0;
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

    /** The uniform bit depth of each channel (the size of each channel in bits), or zero if not all channels have the same bit depth. */
    unsigned uniform_depth = 0;



    PixelBitKit(PixelFormat pixel_format, ChannelOrder channel_order);



    /** Returns the channel count of the specified pixel format. */
    static unsigned determine_channel_count(PixelFormat pixel_format);

    /** Returns the number of bytes of one pixel of the specified format. */
    static unsigned determine_stride(PixelFormat pixel_format);

    /** Returns the data type of the specified pixel format. */
    static DataType determine_data_type(PixelFormat pixel_format);

}; // struct PixelBitKit



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_PIXELBITKIT_H
