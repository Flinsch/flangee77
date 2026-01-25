#ifndef XL7_GRAPHICS_PIXELFORMAT_H
#define XL7_GRAPHICS_PIXELFORMAT_H

#include <CoreLabs/string.h>



namespace xl7::graphics {



/**
 * Describes the component structure, bit depth, and numeric interpretation of a
 * pixel or texel, independent of the semantic channel ordering. It defines:
 * - The number of components (e.g., R, RG, RGB, RGBA, A-only)
 * - Bit per component (e.g., 1, 4, 5, 6, 8, 10, 11, 16, 32)
 *   and packing scheme for tightly packed formats (e.g., 5:6:5, 10:10:10:2)
 * - Numeric representation (UNORM, SNORM, UINT, SINT, FLOAT)
 *
 * The logical ordering of those components is defined separately by ChannelOrder.
 * PixelFormat and ChannelOrder together fully describe the in-memory layout.
 *
 * Naming conventions:
 * - Components are listed in canonical order (R, G, B, A, X) to describe presence,
 *   not memory byte order.
 * - Numbers indicate the bit widths per component.
 * - A suffix indicates the numeric data type.
 *
 * Numeric data types:
 * - UNORM: Unsigned normalized integer, mapped to [0.0, 1.0].
 *   Common for color channels, masks, and general-purpose textures.
 * - SNORM: Signed normalized integer, mapped to [-1.0, +1.0].
 *   Common for direction vectors such as normals and other signed data.
 * - UINT: Unsigned integer, stored and read as-is.
 *   Used for IDs, bitmasks, counters, and unsigned discrete data.
 * - SINT: Signed integer, stored and read as-is.
 *   Used for signed discrete data, indices, and packed metadata.
 * - FLOAT: IEEE-754 floating point, stored and read as-is.
 *   Used for high-precision data such as depths, HDR colors, and simulation values.
 *
 * Channel layouts and special cases:
 * - X components (e.g., R8G8B8X8_UNORM):
 *   Padding or unused bits without semantic meaning.
 * - Single-channel formats (e.g., R8_*, R16_*, R32_*):
 *   Used for grayscale textures, masks, depth-like data, or scalar fields.
 * - Two-channel formats (e.g., R8G8_*, R16G16_*, R32G32_*):
 *   Common for vector fields, UV maps, and other two-component data.
 * - Three-channel formats (e.g., R8G8B8_*, R32G32B32_*):
 *   Used for RGB color or 3D vector data.
 *   Note: R16G16B16 formats are explicitly omitted because of poor hardware support
 *   due to inefficient alignment.
 * - Four-channel formats (e.g., R8G8B8A8_*, R16G16B16A16_*, R32G32B32A32_*):
 *   Standard RGBA formats for color, HDR, and general-purpose textures.
 * - Packed low-bit formats (e.g., R5G6B5_UNORM, R4G4B4A4_UNORM):
 *   Memory-efficient legacy formats with reduced precision.
 * - R11G11B10_FLOAT:
 *   Compact HDR format using shared exponent-style encoding with a higher dynamic
 *   range than UNORM formats at similar memory cost.
 * - A8_UNORM:
 *   Alpha-only format, commonly used for masks, opacity textures, and font atlases.
 */
enum struct PixelFormat
{
    UNKNOWN,

    R8_UNORM,
    R8_SNORM,
    R8_UINT,
    R8_SINT,

    R16_UNORM,
    R16_SNORM,
    R16_UINT,
    R16_SINT,
    R16_FLOAT,

    R32_UINT,
    R32_SINT,
    R32_FLOAT,

    R8G8_UNORM,
    R8G8_SNORM,
    R8G8_UINT,
    R8G8_SINT,

    R16G16_UNORM,
    R16G16_SNORM,
    R16G16_UINT,
    R16G16_SINT,
    R16G16_FLOAT,

    R32G32_UINT,
    R32G32_SINT,
    R32G32_FLOAT,

    R4G4B4X4_UNORM,
    R5G5B5X1_UNORM,
    R5G6B5_UNORM,

    R8G8B8_UNORM,
    R8G8B8_SNORM,
    R8G8B8_UINT,
    R8G8B8_SINT,

    R8G8B8X8_UNORM,

    R11G11B10_FLOAT,

    // R16G16B16 explicitly not

    R32G32B32_UINT,
    R32G32B32_SINT,
    R32G32B32_FLOAT,

    R4G4B4A4_UNORM,
    R5G5B5A1_UNORM,

    R8G8B8A8_UNORM,
    R8G8B8A8_SNORM,
    R8G8B8A8_UINT,
    R8G8B8A8_SINT,

    R10G10B10A2_UNORM,
    R10G10B10A2_UINT,

    R16G16B16A16_UNORM,
    R16G16B16A16_SNORM,
    R16G16B16A16_UINT,
    R16G16B16A16_SINT,
    R16G16B16A16_FLOAT,

    R32G32B32A32_UINT,
    R32G32B32A32_SINT,
    R32G32B32A32_FLOAT,

    A8_UNORM,
};



} // namespace xl7::graphics



namespace cl7 {
    cl7::u8string to_string(xl7::graphics::PixelFormat pixel_format);
}



#endif // XL7_GRAPHICS_PIXELFORMAT_H
