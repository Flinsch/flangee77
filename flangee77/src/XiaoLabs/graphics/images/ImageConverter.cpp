#include "ImageConverter.h"

#include "../PixelBitKit.h"

#include <CoreLabs/bits.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace images {



    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    static void _pack_color(const Color& color, const PixelBitKit& pbk, cl7::byte_span packed_data)
    {
        assert( pbk.data_type != PixelBitKit::DataType::UNKNOWN );
        assert( packed_data.size() >= static_cast<size_t>( pbk.stride ) );

        void* ptr = packed_data.data();

        if ( pbk.data_type == PixelBitKit::DataType::FLOAT )
        {
            switch ( pbk.pixel_format )
            {
            case PixelFormat::R16_FLOAT:
                ((int16_t*)ptr)[0] = cl7::bits::float_to_half( color.r );
                break;
            case PixelFormat::R32_FLOAT:
                ((float*)ptr)[0] = color.r;
                break;
            case PixelFormat::R16G16_FLOAT:
                ((int16_t*)ptr)[pbk.r.index] = cl7::bits::float_to_half( color.r );
                ((int16_t*)ptr)[pbk.g.index] = cl7::bits::float_to_half( color.g );
                break;
            case PixelFormat::R32G32_FLOAT:
                ((float*)ptr)[pbk.r.index] = color.r;
                ((float*)ptr)[pbk.g.index] = color.g;
                break;
            case PixelFormat::R11G11B10_FLOAT:
                assert( false );
                break;
            case PixelFormat::R32G32B32_FLOAT:
                ((float*)ptr)[pbk.r.index] = color.r;
                ((float*)ptr)[pbk.g.index] = color.g;
                ((float*)ptr)[pbk.b.index] = color.b;
                break;
            case PixelFormat::R16G16B16A16_FLOAT:
                ((int16_t*)ptr)[pbk.r.index] = cl7::bits::float_to_half( color.r );
                ((int16_t*)ptr)[pbk.g.index] = cl7::bits::float_to_half( color.g );
                ((int16_t*)ptr)[pbk.b.index] = cl7::bits::float_to_half( color.b );
                ((int16_t*)ptr)[pbk.a.index] = cl7::bits::float_to_half( color.a );
                break;
            case PixelFormat::R32G32B32A32_FLOAT:
                ((float*)ptr)[pbk.r.index] = color.r;
                ((float*)ptr)[pbk.g.index] = color.g;
                ((float*)ptr)[pbk.b.index] = color.b;
                ((float*)ptr)[pbk.a.index] = color.a;
                break;
            default:
                assert( false );
            }
        }
        else if ( pbk.stride > 4 )
        {
            switch ( pbk.pixel_format )
            {
            case PixelFormat::R32G32_UINT:
            case PixelFormat::R32G32_SINT:
                ((uint32_t*)ptr)[pbk.r.index] = cl7::bits::norm_to_fixed( color.r, 32 );
                ((uint32_t*)ptr)[pbk.g.index] = cl7::bits::norm_to_fixed( color.g, 32 );
                break;

            case PixelFormat::R32G32B32_UINT:
            case PixelFormat::R32G32B32_SINT:
                ((uint32_t*)ptr)[pbk.r.index] = cl7::bits::norm_to_fixed( color.r, 32 );
                ((uint32_t*)ptr)[pbk.g.index] = cl7::bits::norm_to_fixed( color.g, 32 );
                ((uint32_t*)ptr)[pbk.b.index] = cl7::bits::norm_to_fixed( color.b, 32 );
                break;

            case PixelFormat::R16G16B16A16_UNORM:
            case PixelFormat::R16G16B16A16_SNORM:
            case PixelFormat::R16G16B16A16_UINT:
            case PixelFormat::R16G16B16A16_SINT:
                ((uint16_t*)ptr)[pbk.r.index] = cl7::bits::norm_to_fixed( color.r, 16 );
                ((uint16_t*)ptr)[pbk.g.index] = cl7::bits::norm_to_fixed( color.g, 16 );
                ((uint16_t*)ptr)[pbk.b.index] = cl7::bits::norm_to_fixed( color.b, 16 );
                ((uint16_t*)ptr)[pbk.a.index] = cl7::bits::norm_to_fixed( color.a, 16 );
                break;

            case PixelFormat::R32G32B32A32_UINT:
            case PixelFormat::R32G32B32A32_SINT:
                ((uint32_t*)ptr)[pbk.r.index] = cl7::bits::norm_to_fixed( color.r, 32 );
                ((uint32_t*)ptr)[pbk.g.index] = cl7::bits::norm_to_fixed( color.g, 32 );
                ((uint32_t*)ptr)[pbk.b.index] = cl7::bits::norm_to_fixed( color.b, 32 );
                ((uint32_t*)ptr)[pbk.a.index] = cl7::bits::norm_to_fixed( color.a, 32 );
                break;

            default:
                assert( false );
            }
        }
        else
        {
            assert( pbk.stride <= 4 );
#pragma warning( push )
#pragma warning( disable: 6297 ) // Temporarily disable "arithmetic overflow" warning.
            uint32_t value = 
                ((cl7::bits::norm_to_fixed( color.r, pbk.r.depth ) << pbk.r.offset) & pbk.r.mask) |
                ((cl7::bits::norm_to_fixed( color.g, pbk.g.depth ) << pbk.g.offset) & pbk.g.mask) |
                ((cl7::bits::norm_to_fixed( color.b, pbk.b.depth ) << pbk.b.offset) & pbk.b.mask) |
                ((cl7::bits::norm_to_fixed( color.a, pbk.a.depth ) << pbk.a.offset) & pbk.a.mask);
#pragma warning( pop )
            ::memcpy( ptr, &value, static_cast<size_t>( pbk.stride ) );
        }
    }

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    static Color _unpack_color(cl7::byte_view packed_data, const PixelBitKit& pbk)
    {
        assert( pbk.data_type != PixelBitKit::DataType::UNKNOWN );
        assert( packed_data.size() >= static_cast<size_t>( pbk.stride ) );

        const void* ptr = packed_data.data();
        Color color{ 0.0f, 0.0f, 0.0f, 1.0f };

        if ( pbk.data_type == PixelBitKit::DataType::FLOAT )
        {
            switch ( pbk.pixel_format )
            {
            case PixelFormat::R16_FLOAT:
                color.r = color.g = color.b = cl7::bits::half_to_float( ((int16_t*)ptr)[0] );
                break;
            case PixelFormat::R32_FLOAT:
                color.r = color.g = color.b = ((float*)ptr)[0];
                break;
            case PixelFormat::R16G16_FLOAT:
                color.r = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.r.index] );
                color.g = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.g.index] );
                break;
            case PixelFormat::R32G32_FLOAT:
                color.r = ((float*)ptr)[pbk.r.index];
                color.g = ((float*)ptr)[pbk.g.index];
                break;
            case PixelFormat::R11G11B10_FLOAT:
                assert( false );
                break;
            case PixelFormat::R32G32B32_FLOAT:
                color.r = ((float*)ptr)[pbk.r.index];
                color.g = ((float*)ptr)[pbk.g.index];
                color.b = ((float*)ptr)[pbk.b.index];
                break;
            case PixelFormat::R16G16B16A16_FLOAT:
                color.r = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.r.index] );
                color.g = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.g.index] );
                color.b = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.b.index] );
                color.a = cl7::bits::half_to_float( ((int16_t*)ptr)[pbk.a.index] );
                break;
            case PixelFormat::R32G32B32A32_FLOAT:
                color.r = ((float*)ptr)[pbk.r.index];
                color.g = ((float*)ptr)[pbk.g.index];
                color.b = ((float*)ptr)[pbk.b.index];
                color.a = ((float*)ptr)[pbk.a.index];
                break;
            default:
                assert( false );
            }
        }
        else if ( pbk.stride > 4 )
        {
            switch ( pbk.pixel_format )
            {
            case PixelFormat::R32G32_UINT:
            case PixelFormat::R32G32_SINT:
                color.r = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.r.index], 32 );
                color.g = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.g.index], 32 );
                break;

            case PixelFormat::R32G32B32_UINT:
            case PixelFormat::R32G32B32_SINT:
                color.r = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.r.index], 32 );
                color.g = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.g.index], 32 );
                color.b = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.b.index], 32 );
                break;

            case PixelFormat::R16G16B16A16_UNORM:
            case PixelFormat::R16G16B16A16_SNORM:
            case PixelFormat::R16G16B16A16_UINT:
            case PixelFormat::R16G16B16A16_SINT:
                color.r = cl7::bits::fixed_to_norm( ((uint16_t*)ptr)[pbk.r.index], 16 );
                color.g = cl7::bits::fixed_to_norm( ((uint16_t*)ptr)[pbk.g.index], 16 );
                color.b = cl7::bits::fixed_to_norm( ((uint16_t*)ptr)[pbk.b.index], 16 );
                color.a = cl7::bits::fixed_to_norm( ((uint16_t*)ptr)[pbk.a.index], 16 );
                break;

            case PixelFormat::R32G32B32A32_UINT:
            case PixelFormat::R32G32B32A32_SINT:
                color.r = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.r.index], 32 );
                color.g = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.g.index], 32 );
                color.b = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.b.index], 32 );
                color.a = cl7::bits::fixed_to_norm( ((uint32_t*)ptr)[pbk.a.index], 32 );
                break;

            default:
                assert( false );
            }
        }
        else
        {
            assert( pbk.stride <= 4 );
            uint32_t value;
            ::memcpy( &value, ptr, static_cast<size_t>( pbk.stride ) );
            if ( pbk.channel_count == 1 )
            {
                if ( pbk.a.depth > 0 )
                {
                    color.a = cl7::bits::fixed_to_norm( (value & pbk.a.mask) >> pbk.a.offset, pbk.a.depth );
                }
                else
                {
                    assert( pbk.r.depth > 0 );
                    color.r = color.g = color.b = cl7::bits::fixed_to_norm( (value & pbk.r.mask) >> pbk.r.offset, pbk.r.depth );
                }
            }
            else
            {
                if ( pbk.r.depth > 0 ) color.r = cl7::bits::fixed_to_norm( (value & pbk.r.mask) >> pbk.r.offset, pbk.r.depth );
                if ( pbk.g.depth > 0 ) color.g = cl7::bits::fixed_to_norm( (value & pbk.g.mask) >> pbk.g.offset, pbk.g.depth );
                if ( pbk.b.depth > 0 ) color.b = cl7::bits::fixed_to_norm( (value & pbk.b.mask) >> pbk.b.offset, pbk.b.depth );
                if ( pbk.a.depth > 0 ) color.a = cl7::bits::fixed_to_norm( (value & pbk.a.mask) >> pbk.a.offset, pbk.a.depth );
            }
        }

        return color;
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Copies pixel data from one image to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size does not
     * change.
     */
    Image ImageConverter::convert_image(const Image& source_image, PixelFormat pixel_format, ChannelOrder channel_order)
    {
        const Image::Desc source_desc = source_image.get_desc();
        const Image::Desc target_desc = {
            pixel_format,
            channel_order,
            source_image.get_desc().width,
            source_image.get_desc().height,
            source_image.get_desc().depth,
        };

        if ( source_desc.pixel_format == PixelFormat::UNKNOWN || target_desc.pixel_format == PixelFormat::UNKNOWN )
        {
            LOG_WARNING( TEXT("Cannot convert from/to an unknown format.") );
            return {};
        }
        if ( source_desc.pixel_format == PixelFormat::R11G11B10_FLOAT || target_desc.pixel_format == PixelFormat::R11G11B10_FLOAT )
        {
            LOG_WARNING( TEXT("Cannot convert from/to R11G11B10_FLOAT.") );
            return {};
        }

        // If pixel format and channel order are identical,
        // then no conversion is required at all.
        if ( source_desc.pixel_format == target_desc.pixel_format && source_desc.channel_order == target_desc.channel_order )
        {
            // Just copy the data/image.
            return source_image;
        }

        const PixelBitKit source_pbk{ source_desc.pixel_format, source_desc.channel_order };
        const PixelBitKit target_pbk{ target_desc.pixel_format, target_desc.channel_order };

        // Another special case is when there is only one channel with identical
        // bit depths and "coding" (i.e., either float or any fixed/integer format),
        // then no conversion is required either.
        const bool both_float = source_pbk.data_type == PixelBitKit::DataType::FLOAT && target_pbk.data_type == PixelBitKit::DataType::FLOAT;
        const bool both_fixed = source_pbk.data_type != PixelBitKit::DataType::FLOAT && target_pbk.data_type != PixelBitKit::DataType::FLOAT;
        const bool single_similar_channel = (both_fixed || both_float) && source_pbk.channel_count == 1 && target_pbk.channel_count == 1 && source_pbk.uniform_depth == target_pbk.uniform_depth;
        if ( single_similar_channel )
        {
            // In this special case, at least the data can simply be copied 1-to-1,
            // even if the two channels don't overlap (e.g., if it's Red and Alpha
            // or vice versa). We then assume that this is the intended behavior.
            // Why else would you want to do such a conversion anyway?
            assert( source_pbk.stride == target_pbk.stride );
            return Image( target_desc, source_image.get_data() );
        }

        const cl7::byte_vector& source_data = source_image.get_data();
        cl7::byte_vector target_data;

        const size_t source_stride = static_cast<size_t>( source_pbk.stride );
        const size_t target_stride = static_cast<size_t>( target_pbk.stride );

        assert( source_stride > 0 );
        assert( target_stride > 0 );

        assert( source_data.size() % source_stride == 0 );
        const size_t pixel_count = source_data.size() / source_stride;
        target_data.resize( pixel_count * target_stride );

        // For now, we only offer dedicated conversions for all typical formats with
        // 8 bits per channel and for formats that differ only in channel order (the
        // integer formats are treated equally here).
        // We handle all other formats using color packing.
        const bool typical_format = both_fixed && source_pbk.uniform_depth == 8 && target_pbk.uniform_depth == 8;
        const bool similar_format = (both_fixed || both_float) && source_pbk.channel_count == target_pbk.channel_count && (source_pbk.r.depth == target_pbk.r.depth && source_pbk.g.depth == target_pbk.g.depth && source_pbk.b.depth == target_pbk.b.depth && source_pbk.a.depth == target_pbk.a.depth);

        const auto* src_ptr = source_data.data();
        auto* dst_ptr = target_data.data();

        if ( typical_format )
        {
            // "Conversion" of typical formats with 8 bits per channel.
            // "Conversion" is the wrong term, hence in quotation marks,
            // because only some channels have to be swapped.
            // In the case of a single channel (e.g., Red or Alpha),
            // it is mapped to several channels (e.g., A8 to R8G8).
            auto FORMAT_HASH = [](unsigned source_count, unsigned target_count) constexpr -> unsigned {
                assert( source_count <= 4 );
                assert( target_count <= 4 );
                return ((source_count - 1) * 4) | (target_count - 1); // format_hash = [0;15]
            };
            const unsigned format_hash = FORMAT_HASH( source_pbk.channel_count, target_pbk.channel_count );
            switch ( format_hash )
            {
            case FORMAT_HASH( 1, 1 ): // R8/A8
                // Already handled.
                assert( false );
                break;
            case FORMAT_HASH( 1, 2 ): // R8/A8 to R8G8
                assert( source_pbk.stride == 1 );
                assert( target_pbk.stride == 2 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[0];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[0];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 1, 3 ): // R8/A8 to R8G8B8/R8G8B8X8
                assert( source_pbk.stride == 1 );
                assert( target_pbk.stride == 3 || target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[0];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[0];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[0];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 1, 4 ): // R8/A8 to R8G8B8A8
                assert( source_pbk.stride == 1 );
                assert( target_pbk.stride == 4 );
                {
                    const uint8_t a_mask = source_pbk.a.depth > 0 ? 0xff : 0x00;
                    const uint8_t rgb_mask = ~a_mask;
                    for ( size_t i = 0; i < pixel_count; ++i )
                    {
                        ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[0] & rgb_mask;
                        ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[0] & rgb_mask;
                        ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[0] & rgb_mask;
                        ((uint8_t*)dst_ptr)[target_pbk.a.index] = ((uint8_t*)src_ptr)[0] & a_mask | rgb_mask;
                        src_ptr += source_stride;
                        dst_ptr += target_stride;
                    }
                }
                break;
            case FORMAT_HASH( 2, 1 ): // R8G8 to R8/A8
                assert( source_pbk.stride == 2 );
                assert( target_pbk.stride == 1 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[0] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 2, 2 ): // R8G8
                assert( source_pbk.stride == 2 );
                assert( target_pbk.stride == 2 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 2, 3 ): // R8G8 to R8G8B8/R8G8B8X8
                assert( source_pbk.stride == 2 );
                assert( target_pbk.stride == 3 || target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = 0x00;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 2, 4 ): // R8G8 to R8G8B8A8
                assert( source_pbk.stride == 2 );
                assert( target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = 0x00;
                    ((uint8_t*)dst_ptr)[target_pbk.a.index] = 0xff;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 3, 1 ): // R8G8B8/R8G8B8X8 to R8/A8
                assert( source_pbk.stride == 3 || source_pbk.stride == 4 );
                assert( target_pbk.stride == 1 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[0] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 3, 2 ): // R8G8B8/R8G8B8X8 to R8G8
                assert( source_pbk.stride == 3 || source_pbk.stride == 4 );
                assert( target_pbk.stride == 2 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 3, 3 ): // R8G8B8/R8G8B8X8
                assert( source_pbk.stride == 3 || source_pbk.stride == 4 );
                assert( target_pbk.stride == 3 || target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[source_pbk.b.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 3, 4 ): // R8G8B8/R8G8B8X8 to R8G8B8A8
                assert( source_pbk.stride == 3 || source_pbk.stride == 4 );
                assert( target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[source_pbk.b.index];
                    ((uint8_t*)dst_ptr)[target_pbk.a.index] = 0xff;
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 4, 1 ): // R8G8B8A8 to R8/A8
                assert( source_pbk.stride == 4 );
                assert( target_pbk.stride == 1 );
                {
                    const unsigned source_channel_index = target_pbk.a.depth > 0 ? 3 : 0;
                    for ( size_t i = 0; i < pixel_count; ++i )
                    {
                        ((uint8_t*)dst_ptr)[0] = ((uint8_t*)src_ptr)[source_pbk.channels[source_channel_index].index];
                        src_ptr += source_stride;
                        dst_ptr += target_stride;
                    }
                }
                break;
            case FORMAT_HASH( 4, 2 ): // R8G8B8A8 to R8G8
                assert( source_pbk.stride == 4 );
                assert( target_pbk.stride == 2 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 4, 3 ): // R8G8B8A8 to R8G8B8/R8G8B8X8
                assert( source_pbk.stride == 4 );
                assert( target_pbk.stride == 3 || target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[source_pbk.b.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            case FORMAT_HASH( 4, 4 ): // R8G8B8A8
                assert( source_pbk.stride == 4 );
                assert( target_pbk.stride == 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint8_t*)dst_ptr)[target_pbk.r.index] = ((uint8_t*)src_ptr)[source_pbk.r.index];
                    ((uint8_t*)dst_ptr)[target_pbk.g.index] = ((uint8_t*)src_ptr)[source_pbk.g.index];
                    ((uint8_t*)dst_ptr)[target_pbk.b.index] = ((uint8_t*)src_ptr)[source_pbk.b.index];
                    ((uint8_t*)dst_ptr)[target_pbk.a.index] = ((uint8_t*)src_ptr)[source_pbk.a.index];
                    src_ptr += source_stride;
                    dst_ptr += target_stride;
                }
                break;
            default:
                assert( false );
            }
        }
        else if ( similar_format )
        {
            // Conversion of "similar" formats that differ only in channel order
            // (with integer formats treated equally again).
            assert( source_stride == target_stride );
            const unsigned channel_count = source_pbk.channel_count;
            const size_t stride = source_stride;
            if ( stride == 16 ) // R32G32B32A32
            {
                assert( channel_count == 4 );
                assert( source_pbk.uniform_depth == 32 );
                assert( target_pbk.uniform_depth == 32 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint32_t*)dst_ptr)[target_pbk.r.index] = ((uint32_t*)src_ptr)[source_pbk.r.index];
                    ((uint32_t*)dst_ptr)[target_pbk.g.index] = ((uint32_t*)src_ptr)[source_pbk.g.index];
                    ((uint32_t*)dst_ptr)[target_pbk.b.index] = ((uint32_t*)src_ptr)[source_pbk.b.index];
                    ((uint32_t*)dst_ptr)[target_pbk.a.index] = ((uint32_t*)src_ptr)[source_pbk.a.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 12 ) // R32G32B32
            {
                assert( channel_count == 3 );
                assert( source_pbk.uniform_depth == 32 );
                assert( target_pbk.uniform_depth == 32 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint32_t*)dst_ptr)[target_pbk.r.index] = ((uint32_t*)src_ptr)[source_pbk.r.index];
                    ((uint32_t*)dst_ptr)[target_pbk.g.index] = ((uint32_t*)src_ptr)[source_pbk.g.index];
                    ((uint32_t*)dst_ptr)[target_pbk.b.index] = ((uint32_t*)src_ptr)[source_pbk.b.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 8 && channel_count == 4 ) // R16G16B16A16
            {
                assert( source_pbk.uniform_depth == 16 );
                assert( target_pbk.uniform_depth == 16 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint16_t*)dst_ptr)[target_pbk.r.index] = ((uint16_t*)src_ptr)[source_pbk.r.index];
                    ((uint16_t*)dst_ptr)[target_pbk.g.index] = ((uint16_t*)src_ptr)[source_pbk.g.index];
                    ((uint16_t*)dst_ptr)[target_pbk.b.index] = ((uint16_t*)src_ptr)[source_pbk.b.index];
                    ((uint16_t*)dst_ptr)[target_pbk.a.index] = ((uint16_t*)src_ptr)[source_pbk.a.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 8 && channel_count == 2 ) // R32G32
            {
                assert( source_pbk.uniform_depth == 32 );
                assert( target_pbk.uniform_depth == 32 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint32_t*)dst_ptr)[target_pbk.r.index] = ((uint32_t*)src_ptr)[source_pbk.r.index];
                    ((uint32_t*)dst_ptr)[target_pbk.g.index] = ((uint32_t*)src_ptr)[source_pbk.g.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 4 && channel_count == 4 ) // R10G10B10A2
            {
                assert( source_pbk.uniform_depth == 0 );
                assert( target_pbk.uniform_depth == 0 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    const uint32_t value = *((uint32_t*)src_ptr);
                    *((uint32_t*)dst_ptr) =
                        (((value & static_cast<uint32_t>( source_pbk.r.mask )) >> source_pbk.r.offset) << target_pbk.r.offset) |
                        (((value & static_cast<uint32_t>( source_pbk.g.mask )) >> source_pbk.g.offset) << target_pbk.g.offset) |
                        (((value & static_cast<uint32_t>( source_pbk.b.mask )) >> source_pbk.b.offset) << target_pbk.b.offset) |
                        (((value & static_cast<uint32_t>( source_pbk.a.mask )) >> source_pbk.a.offset) << target_pbk.a.offset);
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 4 && channel_count == 2 ) // R16G16
            {
                assert( source_pbk.uniform_depth == 16 );
                assert( target_pbk.uniform_depth == 16 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    ((uint16_t*)dst_ptr)[target_pbk.r.index] = ((uint16_t*)src_ptr)[source_pbk.r.index];
                    ((uint16_t*)dst_ptr)[target_pbk.g.index] = ((uint16_t*)src_ptr)[source_pbk.g.index];
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
            else if ( stride == 4 && channel_count == 1 ) // R32
            {
                // Already handled.
                assert( false );
            }
            else if ( stride == 2 && channel_count == 1 ) // R16
            {
                // Already handled.
                assert( false );
            }
            else // R4G4B4X4, R5G5B5X1, R5G6B5, R4G4B4A4, R5G5B5A1
            {
                assert( stride == 2 );
                assert( channel_count >= 3 && channel_count <= 4 );
                for ( size_t i = 0; i < pixel_count; ++i )
                {
                    const uint16_t value = *((uint16_t*)src_ptr);
                    *((uint16_t*)dst_ptr) =
                        (((value & static_cast<uint16_t>( source_pbk.r.mask )) >> source_pbk.r.offset) << target_pbk.r.offset) |
                        (((value & static_cast<uint16_t>( source_pbk.g.mask )) >> source_pbk.g.offset) << target_pbk.g.offset) |
                        (((value & static_cast<uint16_t>( source_pbk.b.mask )) >> source_pbk.b.offset) << target_pbk.b.offset) |
                        (((value & static_cast<uint16_t>( source_pbk.a.mask )) >> source_pbk.a.offset) << target_pbk.a.offset);
                    src_ptr += stride;
                    dst_ptr += stride;
                }
            }
        }
        else if ( source_pbk.channel_count == 1 && target_pbk.channel_count == 1 )
        {
            // We handle all other formats using color packing. But in the case of
            // only one channel, we pay special attention again, not for performance
            // reasons, but because of the assumed intended semantics: although we
            // also use color packing for the conversion, we explicitly want to
            // allow Red and Alpha to be mixed, for example.
            const unsigned source_channel_index = source_pbk.a.depth > 0 ? 3 : 0;
            const unsigned target_channel_index = target_pbk.a.depth > 0 ? 3 : 0;
            for ( size_t i = 0; i < pixel_count; ++i )
            {
                Color color = _unpack_color( { src_ptr, source_stride }, source_pbk );
                color.get_rgba_ptr()[ target_channel_index ] = color.get_rgba_ptr()[ source_channel_index ];
                _pack_color( color, target_pbk, { dst_ptr, target_stride } );
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else if ( source_pbk.channel_count == 1 )
        {
            // Oh snap, yes, a single source channel should
            // be mapped to multiple target channels.
            assert( target_pbk.channel_count > 1 );
            const float a_factor = source_pbk.a.depth > 0 && target_pbk.a.depth > 0 ? 1.0f : 0.0f;
            const float rgb_factor = 1.0f - a_factor;
            for ( size_t i = 0; i < pixel_count; ++i )
            {
                Color color = _unpack_color( { src_ptr, source_stride }, source_pbk );
                color.r *= rgb_factor;
                color.g *= rgb_factor;
                color.b *= rgb_factor;
                color.a *= a_factor;
                color.a += rgb_factor;
                _pack_color( color, target_pbk, { dst_ptr, target_stride } );
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else if ( target_pbk.channel_count == 1 )
        {
            // Almost there: Reverse case of a single target
            // channel fed from either Red or Alpha.
            assert( source_pbk.channel_count > 1 );
            const unsigned source_channel_index = source_pbk.a.depth > 0 && target_pbk.a.depth > 0 ? 3 : 0;
            const unsigned target_channel_index = target_pbk.a.depth > 0 ? 3 : 0;
            for ( size_t i = 0; i < pixel_count; ++i )
            {
                Color color = _unpack_color( { src_ptr, source_stride }, source_pbk );
                color.get_rgba_ptr()[ target_channel_index ] = color.get_rgba_ptr()[ source_channel_index ];
                _pack_color( color, target_pbk, { dst_ptr, target_stride } );
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }
        else
        {
            // Finally! Now here is the general color packing fallback.
            assert( source_pbk.channel_count > 1 && target_pbk.channel_count > 1 );
            for ( size_t i = 0; i < pixel_count; ++i )
            {
                Color color = _unpack_color( { src_ptr, source_stride }, source_pbk );
                _pack_color( color, target_pbk, { dst_ptr, target_stride } );
                src_ptr += source_stride;
                dst_ptr += target_stride;
            }
        }

        return Image( target_desc, std::move(target_data) );
    }

    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    void ImageConverter::pack_color(const Color& color, PixelFormat pixel_format, ChannelOrder channel_order, cl7::byte_span packed_data)
    {
        if ( pixel_format == PixelFormat::UNKNOWN )
        {
            LOG_WARNING( TEXT("Cannot pack a color to an unknown format.") );
            return;
        }
        if ( pixel_format == PixelFormat::R11G11B10_FLOAT )
        {
            LOG_WARNING( TEXT("Cannot pack a color to R11G11B10_FLOAT.") );
            return;
        }

        const PixelBitKit pbk{ pixel_format, channel_order };

        if ( packed_data.size() < static_cast<size_t>( pbk.stride ) )
        {
            LOG_WARNING( TEXT("The receiving data buffer does not have the minimum required size.") );
            return;
        }

        _pack_color( color, pbk, packed_data );
    }

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    Color ImageConverter::unpack_color(cl7::byte_view packed_data, PixelFormat pixel_format, ChannelOrder channel_order)
    {
        if ( pixel_format == PixelFormat::UNKNOWN )
        {
            LOG_WARNING( TEXT("Cannot unpack a color from an unknown format.") );
            return Color::ZERO;
        }
        if ( pixel_format == PixelFormat::R11G11B10_FLOAT )
        {
            LOG_WARNING( TEXT("Cannot unpack a color from R11G11B10_FLOAT.") );
            return Color::ZERO;
        }

        const PixelBitKit pbk{ pixel_format, channel_order };

        if ( packed_data.size() < static_cast<size_t>( pbk.stride ) )
        {
            LOG_WARNING( TEXT("The source data buffer does not have the expected minimum size.") );
            return Color::ZERO;
        }

        return _unpack_color( packed_data, { pixel_format, channel_order } );
    }



} // namespace images
} // namespace graphics
} // namespace xl7
