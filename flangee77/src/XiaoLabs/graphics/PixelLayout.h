#ifndef XL7_GRAPHICS_PIXELLAYOUT_H
#define XL7_GRAPHICS_PIXELLAYOUT_H

#include "./PixelFormat.h"
#include "./ChannelOrder.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_span.h>



namespace xl7::graphics {



/**
 * Utility describing the bit-level structure and channel metadata of a pixel format
 * in a concrete, machine-usable form. It bridges the gap between the abstract format
 * descriptors (PixelFormat and ChannelOrder) and practical bit manipulation by:
 * - Resolving per-channel bit depths and offsets
 * - Computing per-channel bit masks
 * - Determining channel count and pixel stride
 * - Exposing a uniform interface for channel access and packing/unpacking
 *
 * PixelFormat defines which components exists and how many bits they use, while
 * ChannelOrder defines how those components map to semantic channels (R, G, B, A)
 * in memory. PixelLayout combines both to fully describe the concrete memory layout
 * of a single pixel.
 *
 * Notes:
 * - This utility does not perform color space conversion or numeric normalization;
 *   it only describes the bit-level layout.
 * - Integers vs. normalized vs. floating-point interpretation is conveyed via the
 *   data type and must be handled by higher-level code.
 * - Some formats (e.g., R11G11B10_FLOAT) may require special handling beyond simple
 *   bit masking due to non-IEEE packing.
 */
struct PixelLayout
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
        /** The 0-based logical index of the channel within a pixel. */
        unsigned index;
        /** The bit depth of the channel (the size of the channel, in bits) and main indicator of whether the channel is involved at all (0: absent). */
        unsigned depth;
        /** The offset/shift of the channel within a pixel, in bits. */
        unsigned offset;
        /** The bit mask positioned at the channel's offset within a pixel (bit-depth 1s shifted by offset bits). */
        uint64_t mask;
        /** The "standalone" bit mask of the channel positioned at bit 0 (bit-depth 1s shifted by 0 bits). */
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



    PixelLayout(PixelFormat pixel_format, ChannelOrder channel_order);



    /** Returns the channel count of the specified pixel format. */
    static unsigned determine_channel_count(PixelFormat pixel_format);

    /** Returns the number of bytes of one pixel of the specified format. */
    static unsigned determine_stride(PixelFormat pixel_format);

    /** Returns the data type of the specified pixel format. */
    static DataType determine_data_type(PixelFormat pixel_format);

}; // struct PixelLayout



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_PIXELLAYOUT_H
