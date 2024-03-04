#include "PixelBitKit.h"

#include <array>



namespace xl7 {
namespace graphics {



    PixelBitKit::PixelBitKit(PixelFormat pixel_format, ChannelOrder channel_order)
    {
        ::memset( this, 0, sizeof(PixelBitKit) );

        this->pixel_format = pixel_format;
        this->channel_order = channel_order;

        channel_count = determine_channel_count( pixel_format );
        stride = determine_stride( pixel_format );
        data_type = determine_data_type( pixel_format );

        std::array<unsigned, 4> depths;
        std::array<unsigned, 4> rgba;

        switch ( pixel_format )
        {
        case PixelFormat::UNKNOWN:
            return;

        case PixelFormat::R8_UNORM:
        case PixelFormat::R8_SNORM:
        case PixelFormat::R8_UINT:
        case PixelFormat::R8_SINT:
            assert( channel_count == 1 );
            assert( stride == 1 );
            depths = { 8, 0, 0, 0 };
            break;

        case PixelFormat::R16_UNORM:
        case PixelFormat::R16_SNORM:
        case PixelFormat::R16_UINT:
        case PixelFormat::R16_SINT:
        case PixelFormat::R16_FLOAT:
            assert( channel_count == 1 );
            assert( stride == 2 );
            depths = { 16, 0, 0, 0 };
            break;

        case PixelFormat::R32_UINT:
        case PixelFormat::R32_SINT:
        case PixelFormat::R32_FLOAT:
            assert( channel_count == 1 );
            assert( stride == 4 );
            depths = { 32, 0, 0, 0 };
            break;

        case PixelFormat::R8G8_UNORM:
        case PixelFormat::R8G8_SNORM:
        case PixelFormat::R8G8_UINT:
        case PixelFormat::R8G8_SINT:
            assert( channel_count == 2 );
            assert( stride == 2 );
            depths = { 8, 8, 0, 0 };
            break;

        case PixelFormat::R16G16_UNORM:
        case PixelFormat::R16G16_SNORM:
        case PixelFormat::R16G16_UINT:
        case PixelFormat::R16G16_SINT:
        case PixelFormat::R16G16_FLOAT:
            assert( channel_count == 2 );
            assert( stride == 4 );
            depths = { 16, 16, 0, 0 };
            break;

        case PixelFormat::R32G32_UINT:
        case PixelFormat::R32G32_SINT:
        case PixelFormat::R32G32_FLOAT:
            assert( channel_count == 2 );
            assert( stride == 8 );
            depths = { 32, 32, 0, 0 };
            break;

        case PixelFormat::R4G4B4X4_UNORM:
            assert( channel_count == 3 );
            assert( stride == 2 );
            depths = { 4, 4, 4, 0 };
            break;
        case PixelFormat::R5G5B5X1_UNORM:
            assert( channel_count == 3 );
            assert( stride == 2 );
            depths = { 5, 5, 5, 0 };
            break;
        case PixelFormat::R5G6B5_UNORM:
            assert( channel_count == 3 );
            assert( stride == 2 );
            depths = { 5, 6, 5, 0 };
            break;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            assert( channel_count == 3 );
            assert( stride == 3 );
            depths = { 8, 8, 8, 0 };
            break;

        case PixelFormat::R8G8B8X8_UNORM:
            assert( channel_count == 3 );
            assert( stride == 4 );
            depths = { 8, 8, 8, 0 };
            break;

        case PixelFormat::R11G11B10_FLOAT:
            assert( channel_count == 3 );
            assert( stride == 4 );
            depths = { 11, 11, 10, 0 };
            break;

        case PixelFormat::R32G32B32_UINT:
        case PixelFormat::R32G32B32_SINT:
        case PixelFormat::R32G32B32_FLOAT:
            assert( channel_count == 3 );
            assert( stride == 12 );
            depths = { 32, 32, 32, 0 };
            break;

        case PixelFormat::R4G4B4A4_UNORM:
            assert( channel_count == 4 );
            assert( stride == 2 );
            depths = { 4, 4, 4, 4 };
            break;
        case PixelFormat::R5G5B5A1_UNORM:
            assert( channel_count == 4 );
            assert( stride == 2 );
            depths = { 5, 5, 5, 1 };
            break;

        case PixelFormat::R8G8B8A8_UNORM:
        case PixelFormat::R8G8B8A8_SNORM:
        case PixelFormat::R8G8B8A8_UINT:
        case PixelFormat::R8G8B8A8_SINT:
            assert( channel_count == 4 );
            assert( stride == 4 );
            depths = { 8, 8, 8, 8 };
            break;

        case PixelFormat::R10G10B10A2_UNORM:
        case PixelFormat::R10G10B10A2_UINT:
            assert( channel_count == 4 );
            assert( stride == 4 );
            depths = { 10, 10, 10, 2 };
            break;

        case PixelFormat::R16G16B16A16_UNORM:
        case PixelFormat::R16G16B16A16_SNORM:
        case PixelFormat::R16G16B16A16_UINT:
        case PixelFormat::R16G16B16A16_SINT:
        case PixelFormat::R16G16B16A16_FLOAT:
            assert( channel_count == 4 );
            assert( stride == 8 );
            depths = { 16, 16, 16, 16 };
            break;

        case PixelFormat::R32G32B32A32_UINT:
        case PixelFormat::R32G32B32A32_SINT:
        case PixelFormat::R32G32B32A32_FLOAT:
            assert( channel_count == 4 );
            assert( stride == 16 );
            depths = { 32, 32, 32, 32 };
            break;

        case PixelFormat::A8_UNORM:
            assert( channel_count == 1 );
            assert( stride == 1 );
            depths = { 0, 0, 0, 8 };
            break;

        default:
            assert( false );
        } // switch pixel format

        switch ( channel_order )
        {
        case ChannelOrder::RGBA:
            rgba = { 0, 1, 2, 3 };
            break;
        case ChannelOrder::ARGB:
            rgba = { 3, 0, 1, 2 };
            break;
        case ChannelOrder::ABGR:
            rgba = { 3, 2, 1, 0 };
            break;
        case ChannelOrder::BGRA:
            rgba = { 2, 1, 0, 3 };
            break;
        default:
            assert( false );
        } // switch channel order

        unsigned offset = 0;
        for ( unsigned i : rgba )
        {
            if ( depths[ i ] == 0 )
                continue;

            Channel& channel = channels[ i ];

            channel.depth = depths[ i ];
            channel.offset = offset;
            channel.mask = stride > 8 ? 0Ui64 : (((1Ui64 << channel.depth) - 1Ui64) << channel.offset);

            offset += channel.depth;
        } // for each channel
    }



    /**
     * Returns the channel count of the specified pixel format.
     */
    unsigned PixelBitKit::determine_channel_count(PixelFormat pixel_format)
    {
        unsigned channel_count = 0;

        switch ( pixel_format )
        {
        case PixelFormat::UNKNOWN:
            break;

        case PixelFormat::R8_UNORM:
        case PixelFormat::R8_SNORM:
        case PixelFormat::R8_UINT:
        case PixelFormat::R8_SINT:
            channel_count = 1;
            break;

        case PixelFormat::R16_UNORM:
        case PixelFormat::R16_SNORM:
        case PixelFormat::R16_UINT:
        case PixelFormat::R16_SINT:
        case PixelFormat::R16_FLOAT:
            channel_count = 1;
            break;

        case PixelFormat::R32_UINT:
        case PixelFormat::R32_SINT:
        case PixelFormat::R32_FLOAT:
            channel_count = 1;
            break;

        case PixelFormat::R8G8_UNORM:
        case PixelFormat::R8G8_SNORM:
        case PixelFormat::R8G8_UINT:
        case PixelFormat::R8G8_SINT:
            channel_count = 2;
            break;

        case PixelFormat::R16G16_UNORM:
        case PixelFormat::R16G16_SNORM:
        case PixelFormat::R16G16_UINT:
        case PixelFormat::R16G16_SINT:
        case PixelFormat::R16G16_FLOAT:
            channel_count = 2;
            break;

        case PixelFormat::R32G32_UINT:
        case PixelFormat::R32G32_SINT:
        case PixelFormat::R32G32_FLOAT:
            channel_count = 2;
            break;

        case PixelFormat::R4G4B4X4_UNORM:
            channel_count = 3;
            break;
        case PixelFormat::R5G5B5X1_UNORM:
            channel_count = 3;
            break;
        case PixelFormat::R5G6B5_UNORM:
            channel_count = 3;
            break;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            channel_count = 3;
            break;

        case PixelFormat::R8G8B8X8_UNORM:
            channel_count = 3;
            break;

        case PixelFormat::R11G11B10_FLOAT:
            channel_count = 3;
            break;

        case PixelFormat::R32G32B32_UINT:
        case PixelFormat::R32G32B32_SINT:
        case PixelFormat::R32G32B32_FLOAT:
            channel_count = 3;
            break;

        case PixelFormat::R4G4B4A4_UNORM:
            channel_count = 4;
            break;
        case PixelFormat::R5G5B5A1_UNORM:
            channel_count = 4;
            break;

        case PixelFormat::R8G8B8A8_UNORM:
        case PixelFormat::R8G8B8A8_SNORM:
        case PixelFormat::R8G8B8A8_UINT:
        case PixelFormat::R8G8B8A8_SINT:
            channel_count = 4;
            break;

        case PixelFormat::R10G10B10A2_UNORM:
        case PixelFormat::R10G10B10A2_UINT:
            channel_count = 4;
            break;

        case PixelFormat::R16G16B16A16_UNORM:
        case PixelFormat::R16G16B16A16_SNORM:
        case PixelFormat::R16G16B16A16_UINT:
        case PixelFormat::R16G16B16A16_SINT:
        case PixelFormat::R16G16B16A16_FLOAT:
            channel_count = 4;
            break;

        case PixelFormat::R32G32B32A32_UINT:
        case PixelFormat::R32G32B32A32_SINT:
        case PixelFormat::R32G32B32A32_FLOAT:
            channel_count = 4;
            break;

        case PixelFormat::A8_UNORM:
            channel_count = 1;
            break;

        default:
            assert( false );
        } // switch pixel format

        return channel_count;
    }

    /**
     * Returns the number of bytes of one pixel of the specified format.
     */
    unsigned PixelBitKit::determine_stride(PixelFormat pixel_format)
    {
        unsigned stride = 0;

        switch ( pixel_format )
        {
        case PixelFormat::UNKNOWN:
            break;

        case PixelFormat::R8_UNORM:
        case PixelFormat::R8_SNORM:
        case PixelFormat::R8_UINT:
        case PixelFormat::R8_SINT:
            stride = 1;
            break;

        case PixelFormat::R16_UNORM:
        case PixelFormat::R16_SNORM:
        case PixelFormat::R16_UINT:
        case PixelFormat::R16_SINT:
        case PixelFormat::R16_FLOAT:
            stride = 2;
            break;

        case PixelFormat::R32_UINT:
        case PixelFormat::R32_SINT:
        case PixelFormat::R32_FLOAT:
            stride = 4;
            break;

        case PixelFormat::R8G8_UNORM:
        case PixelFormat::R8G8_SNORM:
        case PixelFormat::R8G8_UINT:
        case PixelFormat::R8G8_SINT:
            stride = 2;
            break;

        case PixelFormat::R16G16_UNORM:
        case PixelFormat::R16G16_SNORM:
        case PixelFormat::R16G16_UINT:
        case PixelFormat::R16G16_SINT:
        case PixelFormat::R16G16_FLOAT:
            stride = 4;
            break;

        case PixelFormat::R32G32_UINT:
        case PixelFormat::R32G32_SINT:
        case PixelFormat::R32G32_FLOAT:
            stride = 8;
            break;

        case PixelFormat::R4G4B4X4_UNORM:
            stride = 2;
            break;
        case PixelFormat::R5G5B5X1_UNORM:
            stride = 2;
            break;
        case PixelFormat::R5G6B5_UNORM:
            stride = 2;
            break;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            stride = 3;
            break;

        case PixelFormat::R8G8B8X8_UNORM:
            stride = 4;
            break;

        case PixelFormat::R11G11B10_FLOAT:
            stride = 4;
            break;

        case PixelFormat::R32G32B32_UINT:
        case PixelFormat::R32G32B32_SINT:
        case PixelFormat::R32G32B32_FLOAT:
            stride = 12;
            break;

        case PixelFormat::R4G4B4A4_UNORM:
            stride = 2;
            break;
        case PixelFormat::R5G5B5A1_UNORM:
            stride = 2;
            break;

        case PixelFormat::R8G8B8A8_UNORM:
        case PixelFormat::R8G8B8A8_SNORM:
        case PixelFormat::R8G8B8A8_UINT:
        case PixelFormat::R8G8B8A8_SINT:
            stride = 4;
            break;

        case PixelFormat::R10G10B10A2_UNORM:
        case PixelFormat::R10G10B10A2_UINT:
            stride = 4;
            break;

        case PixelFormat::R16G16B16A16_UNORM:
        case PixelFormat::R16G16B16A16_SNORM:
        case PixelFormat::R16G16B16A16_UINT:
        case PixelFormat::R16G16B16A16_SINT:
        case PixelFormat::R16G16B16A16_FLOAT:
            stride = 8;
            break;

        case PixelFormat::R32G32B32A32_UINT:
        case PixelFormat::R32G32B32A32_SINT:
        case PixelFormat::R32G32B32A32_FLOAT:
            stride = 16;
            break;

        case PixelFormat::A8_UNORM:
            stride = 1;
            break;

        default:
            assert( false );
        } // switch pixel format

        return stride;
    }

    /**
     * Returns the data type of the specified pixel format.
     */
    PixelBitKit::DataType PixelBitKit::determine_data_type(PixelFormat pixel_format)
    {
        DataType data_type = DataType::UNKNOWN;

        switch ( pixel_format )
        {
            case PixelFormat::UNKNOWN:
                break;

            case PixelFormat::R8_UNORM:
            case PixelFormat::R16_UNORM:
            case PixelFormat::R8G8_UNORM:
            case PixelFormat::R16G16_UNORM:
            case PixelFormat::R4G4B4X4_UNORM:
            case PixelFormat::R5G5B5X1_UNORM:
            case PixelFormat::R5G6B5_UNORM:
            case PixelFormat::R8G8B8_UNORM:
            case PixelFormat::R8G8B8X8_UNORM:
            case PixelFormat::R4G4B4A4_UNORM:
            case PixelFormat::R5G5B5A1_UNORM:
            case PixelFormat::R8G8B8A8_UNORM:
            case PixelFormat::R10G10B10A2_UNORM:
            case PixelFormat::R16G16B16A16_UNORM:
            case PixelFormat::A8_UNORM:
                data_type = DataType::UNORM;
                break;

            case PixelFormat::R8_SNORM:
            case PixelFormat::R16_SNORM:
            case PixelFormat::R8G8_SNORM:
            case PixelFormat::R16G16_SNORM:
            case PixelFormat::R8G8B8_SNORM:
            case PixelFormat::R8G8B8A8_SNORM:
            case PixelFormat::R16G16B16A16_SNORM:
                data_type = DataType::SNORM;
                break;

            case PixelFormat::R8_UINT:
            case PixelFormat::R16_UINT:
            case PixelFormat::R32_UINT:
            case PixelFormat::R8G8_UINT:
            case PixelFormat::R16G16_UINT:
            case PixelFormat::R32G32_UINT:
            case PixelFormat::R8G8B8_UINT:
            case PixelFormat::R32G32B32_UINT:
            case PixelFormat::R8G8B8A8_UINT:
            case PixelFormat::R10G10B10A2_UINT:
            case PixelFormat::R16G16B16A16_UINT:
            case PixelFormat::R32G32B32A32_UINT:
                data_type = DataType::UINT;
                break;

            case PixelFormat::R8_SINT:
            case PixelFormat::R16_SINT:
            case PixelFormat::R32_SINT:
            case PixelFormat::R8G8_SINT:
            case PixelFormat::R16G16_SINT:
            case PixelFormat::R32G32_SINT:
            case PixelFormat::R8G8B8_SINT:
            case PixelFormat::R32G32B32_SINT:
            case PixelFormat::R8G8B8A8_SINT:
            case PixelFormat::R16G16B16A16_SINT:
            case PixelFormat::R32G32B32A32_SINT:
                data_type = DataType::SINT;
                break;

            case PixelFormat::R16_FLOAT:
            case PixelFormat::R32_FLOAT:
            case PixelFormat::R16G16_FLOAT:
            case PixelFormat::R32G32_FLOAT:
            case PixelFormat::R11G11B10_FLOAT:
            case PixelFormat::R32G32B32_FLOAT:
            case PixelFormat::R16G16B16A16_FLOAT:
            case PixelFormat::R32G32B32A32_FLOAT:
                data_type = DataType::FLOAT;
                break;

            default:
                assert( false );
        } // switch pixel format

        return data_type;
    }



} // namespace graphics
} // namespace xl7
