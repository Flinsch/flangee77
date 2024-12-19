#include "ImageProcessor.h"

#include <CoreLabs/bits.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    void ImageProcessor::pack_color(const Color& color, PixelFormat pixel_format, ChannelOrder channel_order, cl7::byte_span packed_data)
    {
        if (pixel_format == PixelFormat::UNKNOWN)
        {
            LOG_WARNING(TEXT("Cannot pack a color to an unknown format."));
            return;
        }
        if (pixel_format == PixelFormat::R11G11B10_FLOAT)
        {
            LOG_WARNING(TEXT("Cannot pack a color to R11G11B10_FLOAT."));
            return;
        }

        const PixelBitKit pbk{pixel_format, channel_order};

        if (packed_data.size() < static_cast<size_t>(pbk.stride))
        {
            LOG_WARNING(TEXT("The receiving data buffer does not have the minimum required size."));
            return;
        }

        _pack_color(color, pbk, packed_data);
    }

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    Color ImageProcessor::unpack_color(cl7::byte_view packed_data, PixelFormat pixel_format, ChannelOrder channel_order)
    {
        if (pixel_format == PixelFormat::UNKNOWN)
        {
            LOG_WARNING(TEXT("Cannot unpack a color from an unknown format."));
            return Color::ZERO;
        }
        if (pixel_format == PixelFormat::R11G11B10_FLOAT)
        {
            LOG_WARNING(TEXT("Cannot unpack a color from R11G11B10_FLOAT."));
            return Color::ZERO;
        }

        const PixelBitKit pbk{pixel_format, channel_order};

        if (packed_data.size() < static_cast<size_t>(pbk.stride))
        {
            LOG_WARNING(TEXT("The source data buffer does not have the expected minimum size."));
            return Color::ZERO;
        }

        return _unpack_color(packed_data, {pixel_format, channel_order});
    }



    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    void ImageProcessor::_pack_color(const Color& color, const PixelBitKit& pbk, cl7::byte_span packed_data)
    {
        assert(pbk.data_type != PixelBitKit::DataType::UNKNOWN);
        assert(packed_data.size() >= static_cast<size_t>(pbk.stride));

        void* ptr = packed_data.data();

        if (pbk.data_type == PixelBitKit::DataType::FLOAT)
        {
            switch (pbk.pixel_format)
            {
            case PixelFormat::R16_FLOAT:
                static_cast<uint16_t*>(ptr)[0] = cl7::bits::float_to_half(color.r);
                break;
            case PixelFormat::R32_FLOAT:
                static_cast<float*>(ptr)[0] = color.r;
                break;
            case PixelFormat::R16G16_FLOAT:
                static_cast<uint16_t*>(ptr)[pbk.r.index] = cl7::bits::float_to_half(color.r);
                static_cast<uint16_t*>(ptr)[pbk.g.index] = cl7::bits::float_to_half(color.g);
                break;
            case PixelFormat::R32G32_FLOAT:
                static_cast<float*>(ptr)[pbk.r.index] = color.r;
                static_cast<float*>(ptr)[pbk.g.index] = color.g;
                break;
            case PixelFormat::R11G11B10_FLOAT:
                assert(false);
                break;
            case PixelFormat::R32G32B32_FLOAT:
                static_cast<float*>(ptr)[pbk.r.index] = color.r;
                static_cast<float*>(ptr)[pbk.g.index] = color.g;
                static_cast<float*>(ptr)[pbk.b.index] = color.b;
                break;
            case PixelFormat::R16G16B16A16_FLOAT:
                static_cast<uint16_t*>(ptr)[pbk.r.index] = cl7::bits::float_to_half(color.r);
                static_cast<uint16_t*>(ptr)[pbk.g.index] = cl7::bits::float_to_half(color.g);
                static_cast<uint16_t*>(ptr)[pbk.b.index] = cl7::bits::float_to_half(color.b);
                static_cast<uint16_t*>(ptr)[pbk.a.index] = cl7::bits::float_to_half(color.a);
                break;
            case PixelFormat::R32G32B32A32_FLOAT:
                static_cast<float*>(ptr)[pbk.r.index] = color.r;
                static_cast<float*>(ptr)[pbk.g.index] = color.g;
                static_cast<float*>(ptr)[pbk.b.index] = color.b;
                static_cast<float*>(ptr)[pbk.a.index] = color.a;
                break;
            default:
                assert(false);
            }
        }
        else if (pbk.stride > 4)
        {
            switch (pbk.pixel_format)
            {
            case PixelFormat::R32G32_UINT:
            case PixelFormat::R32G32_SINT:
                static_cast<uint32_t*>(ptr)[pbk.r.index] = cl7::bits::norm_to_fixed(color.r, 32);
                static_cast<uint32_t*>(ptr)[pbk.g.index] = cl7::bits::norm_to_fixed(color.g, 32);
                break;

            case PixelFormat::R32G32B32_UINT:
            case PixelFormat::R32G32B32_SINT:
                static_cast<uint32_t*>(ptr)[pbk.r.index] = cl7::bits::norm_to_fixed(color.r, 32);
                static_cast<uint32_t*>(ptr)[pbk.g.index] = cl7::bits::norm_to_fixed(color.g, 32);
                static_cast<uint32_t*>(ptr)[pbk.b.index] = cl7::bits::norm_to_fixed(color.b, 32);
                break;

            case PixelFormat::R16G16B16A16_UNORM:
            case PixelFormat::R16G16B16A16_SNORM:
            case PixelFormat::R16G16B16A16_UINT:
            case PixelFormat::R16G16B16A16_SINT:
                static_cast<uint16_t*>(ptr)[pbk.r.index] = cl7::bits::norm_to_fixed(color.r, 16);
                static_cast<uint16_t*>(ptr)[pbk.g.index] = cl7::bits::norm_to_fixed(color.g, 16);
                static_cast<uint16_t*>(ptr)[pbk.b.index] = cl7::bits::norm_to_fixed(color.b, 16);
                static_cast<uint16_t*>(ptr)[pbk.a.index] = cl7::bits::norm_to_fixed(color.a, 16);
                break;

            case PixelFormat::R32G32B32A32_UINT:
            case PixelFormat::R32G32B32A32_SINT:
                static_cast<uint32_t*>(ptr)[pbk.r.index] = cl7::bits::norm_to_fixed(color.r, 32);
                static_cast<uint32_t*>(ptr)[pbk.g.index] = cl7::bits::norm_to_fixed(color.g, 32);
                static_cast<uint32_t*>(ptr)[pbk.b.index] = cl7::bits::norm_to_fixed(color.b, 32);
                static_cast<uint32_t*>(ptr)[pbk.a.index] = cl7::bits::norm_to_fixed(color.a, 32);
                break;

            default:
                assert(false);
            }
        }
        else
        {
            assert(pbk.stride <= 4);
#pragma warning(push)
#pragma warning(disable: 6297) // Temporarily disable "arithmetic overflow" warning.
            uint32_t value = 
                ((cl7::bits::norm_to_fixed(color.r, pbk.r.depth) << pbk.r.offset) & pbk.r.mask) |
                ((cl7::bits::norm_to_fixed(color.g, pbk.g.depth) << pbk.g.offset) & pbk.g.mask) |
                ((cl7::bits::norm_to_fixed(color.b, pbk.b.depth) << pbk.b.offset) & pbk.b.mask) |
                ((cl7::bits::norm_to_fixed(color.a, pbk.a.depth) << pbk.a.offset) & pbk.a.mask);
#pragma warning(pop)
            ::memcpy(ptr, &value, static_cast<size_t>(pbk.stride));
        }
    }

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    Color ImageProcessor::_unpack_color(cl7::byte_view packed_data, const PixelBitKit& pbk)
    {
        assert(pbk.data_type != PixelBitKit::DataType::UNKNOWN);
        assert(packed_data.size() >= static_cast<size_t>(pbk.stride));

        const void* ptr = packed_data.data();
        Color color{0.0f, 0.0f, 0.0f, 1.0f};

        if (pbk.data_type == PixelBitKit::DataType::FLOAT)
        {
            switch (pbk.pixel_format)
            {
            case PixelFormat::R16_FLOAT:
                color.r = color.g = color.b = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[0]);
                break;
            case PixelFormat::R32_FLOAT:
                color.r = color.g = color.b = static_cast<const float*>(ptr)[0];
                break;
            case PixelFormat::R16G16_FLOAT:
                color.r = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.r.index]);
                color.g = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.g.index]);
                break;
            case PixelFormat::R32G32_FLOAT:
                color.r = static_cast<const float*>(ptr)[pbk.r.index];
                color.g = static_cast<const float*>(ptr)[pbk.g.index];
                break;
            case PixelFormat::R11G11B10_FLOAT:
                assert(false);
                break;
            case PixelFormat::R32G32B32_FLOAT:
                color.r = static_cast<const float*>(ptr)[pbk.r.index];
                color.g = static_cast<const float*>(ptr)[pbk.g.index];
                color.b = static_cast<const float*>(ptr)[pbk.b.index];
                break;
            case PixelFormat::R16G16B16A16_FLOAT:
                color.r = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.r.index]);
                color.g = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.g.index]);
                color.b = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.b.index]);
                color.a = cl7::bits::half_to_float(static_cast<const int16_t*>(ptr)[pbk.a.index]);
                break;
            case PixelFormat::R32G32B32A32_FLOAT:
                color.r = static_cast<const float*>(ptr)[pbk.r.index];
                color.g = static_cast<const float*>(ptr)[pbk.g.index];
                color.b = static_cast<const float*>(ptr)[pbk.b.index];
                color.a = static_cast<const float*>(ptr)[pbk.a.index];
                break;
            default:
                assert(false);
            }
        }
        else if (pbk.stride > 4)
        {
            switch (pbk.pixel_format)
            {
            case PixelFormat::R32G32_UINT:
            case PixelFormat::R32G32_SINT:
                color.r = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.r.index], 32);
                color.g = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.g.index], 32);
                break;

            case PixelFormat::R32G32B32_UINT:
            case PixelFormat::R32G32B32_SINT:
                color.r = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.r.index], 32);
                color.g = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.g.index], 32);
                color.b = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.b.index], 32);
                break;

            case PixelFormat::R16G16B16A16_UNORM:
            case PixelFormat::R16G16B16A16_SNORM:
            case PixelFormat::R16G16B16A16_UINT:
            case PixelFormat::R16G16B16A16_SINT:
                color.r = cl7::bits::fixed_to_norm(static_cast<const uint16_t*>(ptr)[pbk.r.index], 16);
                color.g = cl7::bits::fixed_to_norm(static_cast<const uint16_t*>(ptr)[pbk.g.index], 16);
                color.b = cl7::bits::fixed_to_norm(static_cast<const uint16_t*>(ptr)[pbk.b.index], 16);
                color.a = cl7::bits::fixed_to_norm(static_cast<const uint16_t*>(ptr)[pbk.a.index], 16);
                break;

            case PixelFormat::R32G32B32A32_UINT:
            case PixelFormat::R32G32B32A32_SINT:
                color.r = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.r.index], 32);
                color.g = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.g.index], 32);
                color.b = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.b.index], 32);
                color.a = cl7::bits::fixed_to_norm(static_cast<const uint32_t*>(ptr)[pbk.a.index], 32);
                break;

            default:
                assert(false);
            }
        }
        else
        {
            assert(pbk.stride <= 4);
            uint32_t value;
            ::memcpy(&value, ptr, static_cast<size_t>(pbk.stride));
            if (pbk.channel_count == 1)
            {
                if (pbk.a.depth > 0)
                {
                    color.a = cl7::bits::fixed_to_norm((value & pbk.a.mask) >> pbk.a.offset, pbk.a.depth);
                }
                else
                {
                    assert(pbk.r.depth > 0);
                    color.r = color.g = color.b = cl7::bits::fixed_to_norm((value & pbk.r.mask) >> pbk.r.offset, pbk.r.depth);
                }
            }
            else
            {
                if (pbk.r.depth > 0) color.r = cl7::bits::fixed_to_norm((value & pbk.r.mask) >> pbk.r.offset, pbk.r.depth);
                if (pbk.g.depth > 0) color.g = cl7::bits::fixed_to_norm((value & pbk.g.mask) >> pbk.g.offset, pbk.g.depth);
                if (pbk.b.depth > 0) color.b = cl7::bits::fixed_to_norm((value & pbk.b.mask) >> pbk.b.offset, pbk.b.depth);
                if (pbk.a.depth > 0) color.a = cl7::bits::fixed_to_norm((value & pbk.a.mask) >> pbk.a.offset, pbk.a.depth);
            }
        }

        return color;
    }



} // namespace xl7::graphics::images
