#include "ImageResizer.h"

#include <CoreLabs/bits.h>

#include <CoreLabs/logging.h>

#include <cmath>



namespace xl7 {
namespace graphics {
namespace images {



    template <size_t stride>
    static cl7::byte_vector _nearest_neighbor(const Image& source_image, unsigned width, unsigned height, unsigned depth)
    {
        const size_t source_width = static_cast<size_t>( source_image.get_width() );
        const size_t source_height = static_cast<size_t>( source_image.get_height() );
        const size_t source_depth = static_cast<size_t>( source_image.get_depth() );

        const size_t target_width = static_cast<size_t>( width );
        const size_t target_height = static_cast<size_t>( height );
        const size_t target_depth = static_cast<size_t>( depth );

        cl7::byte_view source_data = source_image.get_data();
        assert( source_data.size() == source_width * source_height * source_depth * stride );
        cl7::byte_vector target_data( target_width * target_height * target_depth * stride );

        auto* dst_ptr = target_data.data();

        const size_t src_line_pitch = source_width * stride;
        const size_t src_slice_pitch = source_height * src_line_pitch;

        constexpr unsigned shift = 32;

        const uint64_t step_x64 = (static_cast<uint64_t>( source_width ) << shift) / target_width;
        const uint64_t step_y64 = (static_cast<uint64_t>( source_height ) << shift) / target_height;
        const uint64_t step_z64 = (static_cast<uint64_t>( source_depth ) << shift) / target_depth;

        // Initialize with half step.
        uint64_t src_z64 = (step_z64 >> 1) - 1;
        for ( size_t dst_z = 0; dst_z < target_depth; ++dst_z, src_z64 += step_z64 )
        {
            //const size_t src_z = dst_z * source_depth / target_depth;
            const size_t src_z = static_cast<size_t>( src_z64 >> shift );
            const size_t ofs_z = src_z * src_slice_pitch;

            uint64_t src_y64 = (step_y64 >> 1) - 1;
            for ( size_t dst_y = 0; dst_y < target_height; ++dst_y, src_y64 += step_y64 )
            {
                //const size_t src_y = dst_y * source_height / target_height;
                const size_t src_y = static_cast<size_t>( src_y64 >> shift );
                const size_t ofs_y = src_y * src_line_pitch;

                uint64_t src_x64 = (step_x64 >> 1) - 1;
                for ( size_t dst_x = 0; dst_x < target_width; ++dst_x, src_x64 += step_x64 )
                {
                    //const size_t src_x = dst_x * source_width / target_width;
                    const size_t src_x = static_cast<size_t>( src_x64 >> shift );
                    const size_t ofs_x = src_x * stride;

                    const auto* src_ptr = source_data.data() + (ofs_x + ofs_y + ofs_z);
                    ::memcpy( dst_ptr, src_ptr, stride );

                    dst_ptr += stride;
                } // dst_x
            } // dst_y
        } // dst_z

        return std::move(target_data);
    }

    template <size_t stride>
    static cl7::byte_vector _mipmap1_u8(const Image& source_image)
    {
        assert( source_image.get_width() % 2 == 0 );
        assert( source_image.get_height() == 1 );
        assert( source_image.get_depth() == 1 );

        const size_t source_width = static_cast<size_t>( source_image.get_width() );
        const size_t source_height = static_cast<size_t>( source_image.get_height() );
        const size_t source_depth = static_cast<size_t>( source_image.get_depth() );

        const size_t target_width = static_cast<size_t>( source_width / 2 );
        const size_t target_height = 1;
        const size_t target_depth = 1;

        cl7::byte_view source_data = source_image.get_data();
        assert( source_data.size() == source_width * source_height * source_depth * stride );
        cl7::byte_vector target_data( target_width * target_height * target_depth * stride );

        const uint8_t* src_ptr = (const uint8_t*)source_data.data();
        uint8_t* dst_ptr = (uint8_t*)target_data.data();

        for ( size_t dst_x = 0; dst_x < target_width; ++dst_x )
        {
            for ( size_t k = 0; k < stride; ++k )
            {
                dst_ptr[ k ] = (
                        src_ptr[ k ] +
                        src_ptr[ k + stride ] +
                        1 ) / 2;
            }

            dst_ptr += stride;
            src_ptr += 2 * stride; // Skip one pixel
        } // dst_x

        return std::move(target_data);
    }

    template <size_t stride>
    static cl7::byte_vector _mipmap2_u8(const Image& source_image)
    {
        assert( source_image.get_width() % 2 == 0 );
        assert( source_image.get_height() % 2 == 0 );
        assert( source_image.get_depth() == 1 );

        const size_t source_width = static_cast<size_t>( source_image.get_width() );
        const size_t source_height = static_cast<size_t>( source_image.get_height() );
        const size_t source_depth = static_cast<size_t>( source_image.get_depth() );

        const size_t target_width = static_cast<size_t>( source_width / 2 );
        const size_t target_height = static_cast<size_t>( source_height / 2 );
        const size_t target_depth = 1;

        cl7::byte_view source_data = source_image.get_data();
        assert( source_data.size() == source_width * source_height * source_depth * stride );
        cl7::byte_vector target_data( target_width * target_height * target_depth * stride );

        const uint8_t* src_ptr = (const uint8_t*)source_data.data();
        uint8_t* dst_ptr = (uint8_t*)target_data.data();

        const size_t src_line_pitch = source_width * stride;

        for ( size_t dst_y = 0; dst_y < target_height; ++dst_y )
        {
            for ( size_t dst_x = 0; dst_x < target_width; ++dst_x )
            {
                for ( size_t k = 0; k < stride; ++k )
                {
                    dst_ptr[ k ] = (
                        src_ptr[ k ] +
                        src_ptr[ k + stride ] +
                        src_ptr[ k + src_line_pitch ] +
                        src_ptr[ k + src_line_pitch + stride ] +
                        2 ) / 4;
                }

                dst_ptr += stride;
                src_ptr += 2 * stride; // Skip one pixel
            } // dst_x
            src_ptr += src_line_pitch; // Skip one line
        } // dst_y

        return std::move(target_data);
    }

    template <size_t stride>
    static cl7::byte_vector _mipmap3_u8(const Image& source_image)
    {
        assert( source_image.get_width() % 2 == 0 );
        assert( source_image.get_height() % 2 == 0 );
        assert( source_image.get_depth() % 2 == 0 );

        const size_t source_width = static_cast<size_t>( source_image.get_width() );
        const size_t source_height = static_cast<size_t>( source_image.get_height() );
        const size_t source_depth = static_cast<size_t>( source_image.get_depth() );

        const size_t target_width = static_cast<size_t>( source_width / 2 );
        const size_t target_height = static_cast<size_t>( source_height / 2 );
        const size_t target_depth = static_cast<size_t>( source_depth / 2 );

        cl7::byte_view source_data = source_image.get_data();
        assert( source_data.size() == source_width * source_height * source_depth * stride );
        cl7::byte_vector target_data( target_width * target_height * target_depth * stride );

        const uint8_t* src_ptr = (const uint8_t*)source_data.data();
        uint8_t* dst_ptr = (uint8_t*)target_data.data();

        const size_t src_line_pitch = source_width * stride;
        const size_t src_slice_pitch = source_height * src_line_pitch;

        for ( size_t dst_z = 0; dst_z < target_depth; ++dst_z )
        {
            for ( size_t dst_y = 0; dst_y < target_height; ++dst_y )
            {
                for ( size_t dst_x = 0; dst_x < target_width; ++dst_x )
                {
                    for ( size_t k = 0; k < stride; ++k )
                    {
                        dst_ptr[ k ] = (
                            src_ptr[ k ] +
                            src_ptr[ k + stride ] +
                            src_ptr[ k + src_line_pitch ] +
                            src_ptr[ k + src_line_pitch + stride ] +
                            src_ptr[ k + src_slice_pitch ] +
                            src_ptr[ k + src_slice_pitch + stride ] +
                            src_ptr[ k + src_slice_pitch + src_line_pitch ] +
                            src_ptr[ k + src_slice_pitch + src_line_pitch + stride ] +
                            4 ) / 8;
                    }

                    dst_ptr += stride;
                    src_ptr += 2 * stride; // Skip one pixel
                } // dst_x
                src_ptr += src_line_pitch; // Skip one line
            } // dst_y
            src_ptr += src_slice_pitch; // Skip one slice
        } // dst_z

        return std::move(target_data);
    }

    static cl7::byte_vector _linear_interpolation(const Image& source_image, unsigned width, unsigned height, unsigned depth)
    {
        const Image::Desc source_desc = source_image.get_desc();

        const PixelBitKit pbk{ source_desc.pixel_format, source_desc.channel_order };
        const size_t stride = static_cast<size_t>( pbk.stride );

        const size_t source_width = static_cast<size_t>( source_image.get_width() );
        const size_t source_height = static_cast<size_t>( source_image.get_height() );
        const size_t source_depth = static_cast<size_t>( source_image.get_depth() );

        const size_t target_width = static_cast<size_t>( width );
        const size_t target_height = static_cast<size_t>( height );
        const size_t target_depth = static_cast<size_t>( depth );

        cl7::byte_view source_data = source_image.get_data();
        assert( source_data.size() == source_width * source_height * source_depth * stride );
        cl7::byte_vector target_data( target_width * target_height * target_depth * stride );

        auto* dst_ptr = target_data.data();

        const size_t src_line_pitch = source_width * stride;
        const size_t src_slice_pitch = source_height * src_line_pitch;

        const float old_width_f = static_cast<float>( source_width );
        const float old_height_f = static_cast<float>( source_height );
        const float old_depth_f = static_cast<float>( source_depth );
        const float new_width_f = static_cast<float>( target_width );
        const float new_height_f = static_cast<float>( target_height );
        const float new_depth_f = static_cast<float>( target_depth );

        const float ratio_x = old_width_f / new_width_f;
        const float ratio_y = old_height_f / new_height_f;
        const float ratio_z = old_depth_f / new_depth_f;

        struct Dim
        {
            float dst0;
            float dst1;
            float src0;
            float src1;
            float map0;
            float map1;

            unsigned ofs;
            unsigned len;

            float w0d;
            float w1d;

            void init(size_t dst_i, float ratio_i, float old_size_f)
            {
                dst0 = static_cast<float>( dst_i );
                dst1 = static_cast<float>( dst_i + 1 );
                src0 = dst0 * ratio_i;
                src1 = dst1 * ratio_i;

                map0 = ::floorf( src0 );
                map1 = ::ceilf( src1 );
                assert( map1 <= old_size_f );

                ofs = static_cast<unsigned>( map0 );
                len = static_cast<unsigned>( map1 - map0 );

                w0d = src0 - map0;
                w1d = map1 - src1;
            }

            float weight(unsigned i)
            {
                float w = 1.0f;
                if ( i == 0 )
                    w -= w0d;
                if ( i + 1 == len )
                    w -= w1d;
                assert( w > 0.0f );
                assert( w <= 1.0f );
                return w;
            }
        } dim_x, dim_y, dim_z;

        for ( size_t dst_z = 0; dst_z < target_depth; ++dst_z )
        {
            dim_z.init( dst_z, ratio_z, old_depth_f );

            for ( size_t dst_y = 0; dst_y < target_height; ++dst_y )
            {
                dim_y.init( dst_y, ratio_y, old_height_f );

                for ( size_t dst_x = 0; dst_x < target_width; ++dst_x )
                {
                    dim_x.init( dst_x, ratio_x, old_width_f );

                    Color color;
                    float W = 0.0f;

                    for ( unsigned zi = 0; zi < dim_z.len; ++zi )
                    {
                        const float wz = dim_z.weight( zi );

                        for ( unsigned yi = 0; yi < dim_y.len; ++yi )
                        {
                            const float wy = dim_y.weight( yi );

                            for ( unsigned xi = 0; xi < dim_x.len; ++xi )
                            {
                                const float wx = dim_x.weight( xi );

                                const float w = wx * wy * wz;

                                const auto* src_ptr = source_data.data() + size_t(dim_z.ofs + zi) * src_slice_pitch + size_t(dim_y.ofs + yi) * src_line_pitch + size_t(dim_x.ofs + xi) * stride;
                                Color tmp = ImageProcessor::_unpack_color( { src_ptr, stride }, pbk );

                                color += w * tmp;
                                W += w;
                            } // xi
                        } // yi
                    } // zi

                    assert( W > 0.0f );
                    color /= W;
                    ImageProcessor::_pack_color( color, pbk, { dst_ptr, stride } );

                    dst_ptr += stride;
                } // dst_x
            } // dst_y
        } // dst_z

        return std::move(target_data);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Resizes an image. Pixel format and channel order do not change.
     */
    Image ImageResizer::resize_image(const Image& source_image, unsigned width, unsigned height, unsigned depth, ResamplingMethod resampling_method)
    {
        const Image::Desc source_desc = source_image.get_desc();
        const Image::Desc target_desc = {
            source_image.get_desc().pixel_format,
            source_image.get_desc().channel_order,
            width,
            height,
            depth,
        };

        if ( source_desc.pixel_format == PixelFormat::UNKNOWN && resampling_method != ResamplingMethod::NearestNeighbor )
        {
            LOG_WARNING( TEXT("Cannot resample an unknown format.") );
            return {};
        }
        if ( source_desc.pixel_format == PixelFormat::R11G11B10_FLOAT && resampling_method != ResamplingMethod::NearestNeighbor )
        {
            LOG_WARNING( TEXT("Cannot resample R11G11B10_FLOAT.") );
            return {};
        }

        // If sizes are identical,
        // there is nothing to do.
        if ( source_desc.width == target_desc.width && source_desc.height == target_desc.height && source_desc.depth == target_desc.depth )
        {
            // Just copy the data/image.
            return source_image;
        }

        const PixelBitKit pbk{ source_desc.pixel_format, source_desc.channel_order };

        // Fast nearest-neighbor.
        if ( resampling_method == ResamplingMethod::NearestNeighbor )
        {
            switch ( pbk.stride )
            {
            case 1: return { target_desc, std::move(_nearest_neighbor<1>( source_image, width, height, depth )) };
            case 2: return { target_desc, std::move(_nearest_neighbor<2>( source_image, width, height, depth )) };
            case 3: return { target_desc, std::move(_nearest_neighbor<3>( source_image, width, height, depth )) };
            case 4: return { target_desc, std::move(_nearest_neighbor<4>( source_image, width, height, depth )) };
            //case 6: return { target_desc, std::move(_nearest_neighbor<6>( source_image, width, height, depth )) };
            case 8: return { target_desc, std::move(_nearest_neighbor<8>( source_image, width, height, depth )) };
            case 12: return { target_desc, std::move(_nearest_neighbor<12>( source_image, width, height, depth )) };
            case 16: return { target_desc, std::move(_nearest_neighbor<16>( source_image, width, height, depth )) };
            default:
                assert( false );
            }
        }

        assert( resampling_method == ResamplingMethod::LinearInterpolation );

        // 1D mipmap with typical format of 8 bits per channel.
        if ( pbk.uniform_depth == 8 && source_desc.width == target_desc.width * 2 && source_desc.height == 1 && target_desc.height == 1 && source_desc.depth == 1 && target_desc.depth == 1 )
        {
            switch ( pbk.stride )
            {
            case 1: return { target_desc, std::move(_mipmap1_u8<1>( source_image )) };
            case 2: return { target_desc, std::move(_mipmap1_u8<2>( source_image )) };
            case 3: return { target_desc, std::move(_mipmap1_u8<3>( source_image )) };
            case 4: return { target_desc, std::move(_mipmap1_u8<4>( source_image )) };
            default:
                assert( false );
            }
        }

        // 2D mipmap with typical format of 8 bits per channel.
        if ( pbk.uniform_depth == 8 && source_desc.width == target_desc.width * 2 && source_desc.height == target_desc.height * 2 && source_desc.depth == 1 && target_desc.depth == 1 )
        {
            switch ( pbk.stride )
            {
            case 1: return { target_desc, std::move(_mipmap2_u8<1>( source_image )) };
            case 2: return { target_desc, std::move(_mipmap2_u8<2>( source_image )) };
            case 3: return { target_desc, std::move(_mipmap2_u8<3>( source_image )) };
            case 4: return { target_desc, std::move(_mipmap2_u8<4>( source_image )) };
            default:
                assert( false );
            }
        }

        // 3D mipmap with typical format of 8 bits per channel.
        if ( pbk.uniform_depth == 8 && source_desc.width == target_desc.width * 2 && source_desc.height == target_desc.height * 2 && source_desc.depth == target_desc.depth * 2 )
        {
            switch ( pbk.stride )
            {
            case 1: return { target_desc, std::move(_mipmap3_u8<1>( source_image )) };
            case 2: return { target_desc, std::move(_mipmap3_u8<2>( source_image )) };
            case 3: return { target_desc, std::move(_mipmap3_u8<3>( source_image )) };
            case 4: return { target_desc, std::move(_mipmap3_u8<4>( source_image )) };
            default:
                assert( false );
            }
        }

        // Handle all other linear interpolation cases using color packing.
        return { target_desc, std::move(_linear_interpolation( source_image, width, height, depth )) };
    }

    /**
     * Creates a mipmap from the specified image.
     */
    Image ImageResizer::create_mipmap(const Image& source_image, ResamplingMethod resampling_method)
    {
        unsigned width = source_image.get_width() / 2;
        unsigned height = source_image.get_height() / 2;
        unsigned depth = source_image.get_depth() / 2;

        return resize_image( source_image, width ? width : 1, height ? height : 1, depth ? depth : 1, resampling_method );
    }



} // namespace images
} // namespace graphics
} // namespace xl7
