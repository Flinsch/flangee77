#include "ImageConverter.h"

#include <CoreLabs/bits.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * Copies pixel data from one image to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size does not
     * change.
     */
    Image ImageConverter::convert_image(const Image& source_image, PixelFormat pixel_format, ChannelOrder channel_order)
    {
        const Image::Desc source_desc = source_image.get_desc();
        const Image::Desc target_desc = {
            .pixel_format = pixel_format,
            .channel_order = channel_order,
            .width = source_image.get_desc().width,
            .height = source_image.get_desc().height,
            .depth = source_image.get_desc().depth,
        };

        if (source_desc.pixel_format == PixelFormat::UNKNOWN || target_desc.pixel_format == PixelFormat::UNKNOWN)
        {
            LOG_WARNING(u8"Cannot convert from/to an unknown format.");
            return {};
        }
        if (source_desc.pixel_format == PixelFormat::R11G11B10_FLOAT || target_desc.pixel_format == PixelFormat::R11G11B10_FLOAT)
        {
            LOG_WARNING(u8"Cannot convert from/to R11G11B10_FLOAT.");
            return {};
        }

        // If pixel format and channel order are identical,
        // then no conversion is required at all.
        if (source_desc.pixel_format == target_desc.pixel_format && source_desc.channel_order == target_desc.channel_order)
        {
            // Just copy the data/image.
            return source_image;
        }

        const PixelLayout source_layout{source_desc.pixel_format, source_desc.channel_order};
        const PixelLayout target_layout{target_desc.pixel_format, target_desc.channel_order};

        // Another special case is when there is only one channel with identical
        // bit depths and "coding" (i.e., either float or any fixed/integer format),
        // then no conversion is required either.
        const bool both_float = source_layout.data_type == PixelLayout::DataType::FLOAT && target_layout.data_type == PixelLayout::DataType::FLOAT;
        const bool both_fixed = source_layout.data_type != PixelLayout::DataType::FLOAT && target_layout.data_type != PixelLayout::DataType::FLOAT;
        const bool single_similar_channel = (both_fixed || both_float) && source_layout.channel_count == 1 && target_layout.channel_count == 1 && source_layout.uniform_depth == target_layout.uniform_depth;
        if (single_similar_channel)
        {
            // In this special case, at least the data can simply be copied 1-to-1,
            // even if the two channels don't overlap (e.g., if it's Red and Alpha
            // or vice versa). We then assume that this is the intended behavior.
            // Why else would you want to do such a conversion anyway?
            assert(source_layout.stride == target_layout.stride);
            return {target_desc, source_image.get_data()};
        }

        cl7::byte_view source_data = source_image.get_data();
        cl7::byte_vector target_data;

        const auto source_stride = static_cast<size_t>(source_layout.stride);
        const auto target_stride = static_cast<size_t>(target_layout.stride);

        assert(source_stride > 0);
        assert(target_stride > 0);

        assert(source_data.size() % source_stride == 0);
        const size_t pixel_count = source_data.size() / source_stride;
        target_data.resize(pixel_count * target_stride);

        // For now, we only offer dedicated conversions for all typical formats with
        // 8 bits per channel and for formats that differ only in channel order (the
        // integer formats are treated equally here).
        // We handle all other formats using color packing.
        const bool typical_format = both_fixed && source_layout.uniform_depth == 8 && target_layout.uniform_depth == 8;
        const bool similar_format = (both_fixed || both_float) && source_layout.channel_count == target_layout.channel_count && (source_layout.r.depth == target_layout.r.depth && source_layout.g.depth == target_layout.g.depth && source_layout.b.depth == target_layout.b.depth && source_layout.a.depth == target_layout.a.depth);

        const auto* src_ptr = source_data.data();
        auto* dst_ptr = target_data.data();

        if (typical_format)
        {
            // "Conversion" of typical formats with 8 bits per channel.
            // "Conversion" is the wrong term, hence in quotation marks,
            // because only some channels have to be swapped.
            // In the case of a single channel (e.g., Red or Alpha),
            // it is mapped to several channels (e.g., A8 to R8G8).
            auto FORMAT_HASH = [](unsigned source_count, unsigned target_count) constexpr -> unsigned {
                assert(source_count <= 4);
                assert(target_count <= 4);
                return ((source_count - 1) * 4) | (target_count - 1); // format_hash = [0;15]
            };
            const unsigned format_hash = FORMAT_HASH(source_layout.channel_count, target_layout.channel_count);
            switch (format_hash)
            {
            case FORMAT_HASH(1, 1): // R8/A8
                // Already handled.
                assert(false);
                break;
            case FORMAT_HASH(1, 2): // R8/A8 to R8G8
                assert(source_layout.stride == 1);
                assert(target_layout.stride == 2);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(1, 3): // R8/A8 to R8G8B8/R8G8B8X8
                assert(source_layout.stride == 1);
                assert(target_layout.stride == 3 || target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(1, 4): // R8/A8 to R8G8B8A8
                assert(source_layout.stride == 1);
                assert(target_layout.stride == 4);
                {
                    const uint8_t a_mask = source_layout.a.depth > 0 ? 0xff : 0x00;
                    const uint8_t rgb_mask = ~a_mask;
                    for (size_t i = 0; i < pixel_count; ++i)
                    {
                        reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0] & rgb_mask;
                        reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0] & rgb_mask;
                        reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0] & rgb_mask;
                        reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.a.index] = reinterpret_cast<const uint8_t*>(src_ptr)[0] & a_mask | rgb_mask;
                        src_ptr += source_stride;
                        dst_ptr += target_stride;
                    }
                }
                break;
            case FORMAT_HASH(2, 1): // R8G8 to R8/A8
                assert(source_layout.stride == 2);
                assert(target_layout.stride == 1);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[0] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(2, 2): // R8G8
                assert(source_layout.stride == 2);
                assert(target_layout.stride == 2);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(2, 3): // R8G8 to R8G8B8/R8G8B8X8
                assert(source_layout.stride == 2);
                assert(target_layout.stride == 3 || target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = 0x00;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(2, 4): // R8G8 to R8G8B8A8
                assert(source_layout.stride == 2);
                assert(target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = 0x00;
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.a.index] = 0xff;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(3, 1): // R8G8B8/R8G8B8X8 to R8/A8
                assert(source_layout.stride == 3 || source_layout.stride == 4);
                assert(target_layout.stride == 1);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[0] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(3, 2): // R8G8B8/R8G8B8X8 to R8G8
                assert(source_layout.stride == 3 || source_layout.stride == 4);
                assert(target_layout.stride == 2);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(3, 3): // R8G8B8/R8G8B8X8
                assert(source_layout.stride == 3 || source_layout.stride == 4);
                assert(target_layout.stride == 3 || target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.b.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(3, 4): // R8G8B8/R8G8B8X8 to R8G8B8A8
                assert(source_layout.stride == 3 || source_layout.stride == 4);
                assert(target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.b.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.a.index] = 0xff;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(4, 1): // R8G8B8A8 to R8/A8
                assert(source_layout.stride == 4);
                assert(target_layout.stride == 1);
                {
                    const unsigned source_channel_index = target_layout.a.depth > 0 ? 3 : 0;
                    for (size_t i = 0; i < pixel_count; ++i)
                    {
                        reinterpret_cast<uint8_t*>(dst_ptr)[0] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.channels[source_channel_index].index];
                        src_ptr += source_stride;
                        dst_ptr += target_stride;
                    }
                }
                break;
            case FORMAT_HASH(4, 2): // R8G8B8A8 to R8G8
                assert(source_layout.stride == 4);
                assert(target_layout.stride == 2);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(4, 3): // R8G8B8A8 to R8G8B8/R8G8B8X8
                assert(source_layout.stride == 4);
                assert(target_layout.stride == 3 || target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.b.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH(4, 4): // R8G8B8A8
                assert(source_layout.stride == 4);
                assert(target_layout.stride == 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.b.index];
                    reinterpret_cast<uint8_t*>(dst_ptr)[target_layout.a.index] = reinterpret_cast<const uint8_t*>(src_ptr)[source_layout.a.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            default:
                assert(false);
            }
        }
        else if (similar_format)
        {
            // Conversion of "similar" formats that differ only in channel order
            // (with integer formats treated equally again).
            assert(source_stride == target_stride);
            const unsigned channel_count = source_layout.channel_count;
            const size_t stride = source_stride;
            if (stride == 16) // R32G32B32A32
            {
                assert(channel_count == 4);
                assert(source_layout.uniform_depth == 32);
                assert(target_layout.uniform_depth == 32);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.b.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.a.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.a.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 12) // R32G32B32
            {
                assert(channel_count == 3);
                assert(source_layout.uniform_depth == 32);
                assert(target_layout.uniform_depth == 32);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.b.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 8 && channel_count == 4) // R16G16B16A16
            {
                assert(source_layout.uniform_depth == 16);
                assert(target_layout.uniform_depth == 16);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.b.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.a.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.a.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 8 && channel_count == 2) // R32G32
            {
                assert(source_layout.uniform_depth == 32);
                assert(target_layout.uniform_depth == 32);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint32_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint32_t*>(src_ptr)[source_layout.g.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 6 && channel_count == 3) // R16G16B16
            {
                assert(source_layout.uniform_depth == 16);
                assert(target_layout.uniform_depth == 16);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.g.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.b.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.b.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 4 && channel_count == 4) // R10G10B10A2
            {
                assert(source_layout.uniform_depth == 0);
                assert(target_layout.uniform_depth == 0);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    const uint32_t value = *reinterpret_cast<const uint32_t*>(src_ptr);
                    *reinterpret_cast<uint32_t*>(dst_ptr) =
                        (((value & static_cast<uint32_t>(source_layout.r.mask)) >> source_layout.r.offset) << target_layout.r.offset) |
                        (((value & static_cast<uint32_t>(source_layout.g.mask)) >> source_layout.g.offset) << target_layout.g.offset) |
                        (((value & static_cast<uint32_t>(source_layout.b.mask)) >> source_layout.b.offset) << target_layout.b.offset) |
                        (((value & static_cast<uint32_t>(source_layout.a.mask)) >> source_layout.a.offset) << target_layout.a.offset);
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 4 && channel_count == 2) // R16G16
            {
                assert(source_layout.uniform_depth == 16);
                assert(target_layout.uniform_depth == 16);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.r.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.r.index];
                    reinterpret_cast<uint16_t*>(dst_ptr)[target_layout.g.index] = reinterpret_cast<const uint16_t*>(src_ptr)[source_layout.g.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if (stride == 4 && channel_count == 1) // R32
            {
                // Already handled.
                assert(false);
            }
            else if (stride == 2 && channel_count == 1) // R16
            {
                // Already handled.
                assert(false);
            }
            else // R4G4B4X4, R5G5B5X1, R5G6B5, R4G4B4A4, R5G5B5A1
            {
                assert(stride == 2);
                assert(channel_count >= 3 && channel_count <= 4);
                for (size_t i = 0; i < pixel_count; ++i)
                {
                    const uint16_t value = *reinterpret_cast<const uint16_t*>(src_ptr);
                    *reinterpret_cast<uint16_t*>(dst_ptr) =
                        (((value & static_cast<uint16_t>(source_layout.r.mask)) >> source_layout.r.offset) << target_layout.r.offset) |
                        (((value & static_cast<uint16_t>(source_layout.g.mask)) >> source_layout.g.offset) << target_layout.g.offset) |
                        (((value & static_cast<uint16_t>(source_layout.b.mask)) >> source_layout.b.offset) << target_layout.b.offset) |
                        (((value & static_cast<uint16_t>(source_layout.a.mask)) >> source_layout.a.offset) << target_layout.a.offset);
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
        }
        else if (source_layout.channel_count == 1 && target_layout.channel_count == 1)
        {
            // We handle all other formats using color packing. But in the case of
            // only one channel, we pay special attention again, not for performance
            // reasons, but because of the assumed intended semantics: although we
            // also use color packing for the conversion, we explicitly want to
            // allow Red and Alpha to be mixed, for example.
            const unsigned source_channel_index = source_layout.a.depth > 0 ? 3 : 0;
            const unsigned target_channel_index = target_layout.a.depth > 0 ? 3 : 0;
            for (size_t i = 0; i < pixel_count; ++i)
            {
                Color color = _unpack_color({src_ptr, source_stride}, source_layout);
                color.get_rgba()[target_channel_index] = color.get_rgba()[source_channel_index];
                _pack_color(color, target_layout, {dst_ptr, target_stride});
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else if (source_layout.channel_count == 1)
        {
            // Oh snap, yes, a single source channel should
            // be mapped to multiple target channels.
            assert(target_layout.channel_count > 1);
            const float a_factor = source_layout.a.depth > 0 && target_layout.a.depth > 0 ? 1.0f : 0.0f;
            const float rgb_factor = 1.0f - a_factor;
            for (size_t i = 0; i < pixel_count; ++i)
            {
                Color color = _unpack_color({src_ptr, source_stride}, source_layout);
                color.r *= rgb_factor;
                color.g *= rgb_factor;
                color.b *= rgb_factor;
                color.a *= a_factor;
                color.a += rgb_factor;
                _pack_color(color, target_layout, {dst_ptr, target_stride});
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else if (target_layout.channel_count == 1)
        {
            // Almost there: Reverse case of a single target
            // channel fed from either Red or Alpha.
            assert(source_layout.channel_count > 1);
            const unsigned source_channel_index = source_layout.a.depth > 0 && target_layout.a.depth > 0 ? 3 : 0;
            const unsigned target_channel_index = target_layout.a.depth > 0 ? 3 : 0;
            for (size_t i = 0; i < pixel_count; ++i)
            {
                Color color = _unpack_color({src_ptr, source_stride}, source_layout);
                color.get_rgba()[target_channel_index] = color.get_rgba()[source_channel_index];
                _pack_color(color, target_layout, {dst_ptr, target_stride});
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else
        {
            // Finally! Now here is the general color packing fallback.
            assert(source_layout.channel_count > 1 && target_layout.channel_count > 1);
            for (size_t i = 0; i < pixel_count; ++i)
            {
                Color color = _unpack_color({src_ptr, source_stride}, source_layout);
                _pack_color(color, target_layout, {dst_ptr, target_stride});
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }

        return {target_desc, std::move(target_data)};
    }



} // namespace xl7::graphics::images
