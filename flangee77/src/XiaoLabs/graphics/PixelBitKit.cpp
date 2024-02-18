#include "PixelBitKit.h"

#include <array>



namespace xl7 {
namespace graphics {



    PixelBitKit::PixelBitKit(PixelFormat pixel_format, ChannelOrder channel_order)
    {
        ::memset( this, 0, sizeof(PixelBitKit) );

        this->pixel_format = pixel_format;
        this->channel_order = channel_order;

        std::array<unsigned, 4> depths;
        std::array<unsigned, 4> rgba;

        switch ( pixel_format )
        {
            case PixelFormat::UNKNOWN:
                return;

            case PixelFormat::R8_UNORM:
            case PixelFormat::R16_UNORM:
            case PixelFormat::R8G8_UNORM:
            case PixelFormat::R16G16_UNORM:
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

        switch ( pixel_format )
        {
        case PixelFormat::UNKNOWN:
            return;

        case PixelFormat::R8_UNORM:
        case PixelFormat::R8_SNORM:
        case PixelFormat::R8_UINT:
        case PixelFormat::R8_SINT:
            channel_count = 1;
            stride = 1;
            depths = { 8, 0, 0, 0 };
            break;

        case PixelFormat::R16_UNORM:
        case PixelFormat::R16_SNORM:
        case PixelFormat::R16_UINT:
        case PixelFormat::R16_SINT:
        case PixelFormat::R16_FLOAT:
            channel_count = 1;
            stride = 2;
            depths = { 16, 0, 0, 0 };
            break;

        case PixelFormat::R32_UINT:
        case PixelFormat::R32_SINT:
        case PixelFormat::R32_FLOAT:
            channel_count = 1;
            stride = 4;
            depths = { 32, 0, 0, 0 };
            break;

        case PixelFormat::R8G8_UNORM:
        case PixelFormat::R8G8_SNORM:
        case PixelFormat::R8G8_UINT:
        case PixelFormat::R8G8_SINT:
            channel_count = 2;
            stride = 2;
            depths = { 8, 8, 0, 0 };
            break;

        case PixelFormat::R16G16_UNORM:
        case PixelFormat::R16G16_SNORM:
        case PixelFormat::R16G16_UINT:
        case PixelFormat::R16G16_SINT:
        case PixelFormat::R16G16_FLOAT:
            channel_count = 2;
            stride = 4;
            depths = { 16, 16, 0, 0 };
            break;

        case PixelFormat::R32G32_UINT:
        case PixelFormat::R32G32_SINT:
        case PixelFormat::R32G32_FLOAT:
            channel_count = 2;
            stride = 8;
            depths = { 32, 32, 0, 0 };
            break;

        case PixelFormat::R5G5B5X1_UNORM:
            channel_count = 3;
            stride = 2;
            depths = { 5, 5, 5, 0 };
            break;
        case PixelFormat::R5G6B5_UNORM:
            channel_count = 3;
            stride = 2;
            depths = { 5, 6, 5, 0 };
            break;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            channel_count = 3;
            stride = 3;
            depths = { 8, 8, 8, 0 };
            break;

        case PixelFormat::R8G8B8X8_UNORM:
            channel_count = 3;
            stride = 4;
            depths = { 8, 8, 8, 0 };
            break;

        case PixelFormat::R11G11B10_FLOAT:
            channel_count = 3;
            stride = 4;
            depths = { 11, 11, 10, 0 };
            break;

        case PixelFormat::R32G32B32_UINT:
        case PixelFormat::R32G32B32_SINT:
        case PixelFormat::R32G32B32_FLOAT:
            channel_count = 3;
            stride = 12;
            depths = { 32, 32, 32, 0 };
            break;

        case PixelFormat::R4G4B4A4_UNORM:
            channel_count = 4;
            stride = 2;
            depths = { 4, 4, 4, 4 };
            break;
        case PixelFormat::R5G5B5A1_UNORM:
            channel_count = 4;
            stride = 2;
            depths = { 5, 5, 5, 1 };
            break;

        case PixelFormat::R8G8B8A8_UNORM:
        case PixelFormat::R8G8B8A8_SNORM:
        case PixelFormat::R8G8B8A8_UINT:
        case PixelFormat::R8G8B8A8_SINT:
            channel_count = 4;
            stride = 4;
            depths = { 8, 8, 8, 8 };
            break;

        case PixelFormat::R10G10B10A2_UNORM:
        case PixelFormat::R10G10B10A2_UINT:
            channel_count = 4;
            stride = 4;
            depths = { 10, 10, 10, 2 };
            break;

        case PixelFormat::R16G16B16A16_UNORM:
        case PixelFormat::R16G16B16A16_SNORM:
        case PixelFormat::R16G16B16A16_UINT:
        case PixelFormat::R16G16B16A16_SINT:
        case PixelFormat::R16G16B16A16_FLOAT:
            channel_count = 4;
            stride = 8;
            depths = { 16, 16, 16, 16 };
            break;

        case PixelFormat::R32G32B32A32_UINT:
        case PixelFormat::R32G32B32A32_SINT:
        case PixelFormat::R32G32B32A32_FLOAT:
            channel_count = 4;
            stride = 16;
            depths = { 32, 32, 32, 32 };
            break;

        case PixelFormat::A8_UNORM:
            channel_count = 1;
            stride = 1;
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



} // namespace graphics
} // namespace xl7
