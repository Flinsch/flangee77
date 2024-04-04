#include "mappings.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace mappings {



    DWORD _d3d_usage_from(resources::ResourceUsage resource_usage)
    {
        // Compare if necessary:
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dusage

        // If we do not specify D3DUSAGE_DYNAMIC, the resource is made static.
        // However, D3DUSAGE_DYNAMIC is only useful when the resource is locked frequently.
        // So "default" should not be interpreted as "dynamic" here,
        // because "static" does not mean "immutable".

        switch ( resource_usage )
        {
        case resources::ResourceUsage::Default:
            return D3DUSAGE_WRITEONLY;
        case resources::ResourceUsage::Immutable:
            return D3DUSAGE_WRITEONLY;
        case resources::ResourceUsage::Dynamic:
            return D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
        default:
            assert( false );
        }

        return 0;
    }

    DWORD _d3d_usage_from(resources::ResourceUsage resource_usage, unsigned mip_levels)
    {
        DWORD d3d_usage = 0;

        // Have a general look at:
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dusage

        // D3DUSAGE_WRITEONLY is not used for textures,
        // only for vertex and index buffers (see above).

        // And "default" usage should not be interpreted as "dynamic" here (see above again).

        switch ( resource_usage )
        {
        case resources::ResourceUsage::Default:
            //d3d_usage |= 0;
            break;
        case resources::ResourceUsage::Immutable:
            //d3d_usage |= 0;
            break;
        case resources::ResourceUsage::Dynamic:
            d3d_usage |= D3DUSAGE_DYNAMIC;
            break;
        default:
            assert( false );
        }

        if ( mip_levels != 1 )
        {
            // D3DUSAGE_AUTOGENMIPMAP is only a hint and does not cause an error when used,
            // even if the device does not support the automatic generation of mipmaps.
            // See here:
            // https://learn.microsoft.com/en-us/windows/win32/direct3d9/automatic-generation-of-mipmaps
            d3d_usage |= D3DUSAGE_AUTOGENMIPMAP;
        }

        return d3d_usage;
        return 0;
    }

    D3DPOOL _d3d_pool_from(resources::ResourceUsage resource_usage)
    {
        if ( resource_usage == resources::ResourceUsage::Dynamic )
            return D3DPOOL_DEFAULT;

        return D3DPOOL_MANAGED;
    }

    D3DFORMAT _d3d_format_from(PixelFormat pixel_format, ChannelOrder channel_order)
    {
        static_assert( static_cast<unsigned>( ChannelOrder::RGBA ) == 0 );
        static_assert( static_cast<unsigned>( ChannelOrder::ARGB ) == 1 );
        static_assert( static_cast<unsigned>( ChannelOrder::ABGR ) == 2 );
        static_assert( static_cast<unsigned>( ChannelOrder::BGRA ) == 3 );

        // Also compare the following overview:
        // https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-legacy-formats

        switch ( pixel_format )
        {
        case PixelFormat::UNKNOWN:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R8_UNORM:
            return D3DFMT_L8;
        case PixelFormat::R8_SNORM:
        case PixelFormat::R8_UINT:
        case PixelFormat::R8_SINT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R16_UNORM:
            return D3DFMT_L16;
        case PixelFormat::R16_SNORM:
        case PixelFormat::R16_UINT:
        case PixelFormat::R16_SINT:
        case PixelFormat::R16_FLOAT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R32_UINT:
        case PixelFormat::R32_SINT:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R32_FLOAT:
            return D3DFMT_R32F;

        case PixelFormat::R8G8_UNORM:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R8G8_SNORM:
            return D3DFMT_V8U8;
        case PixelFormat::R8G8_UINT:
        case PixelFormat::R8G8_SINT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R16G16_UNORM:
            return D3DFMT_G16R16;
        case PixelFormat::R16G16_SNORM:
            return D3DFMT_V16U16;
        case PixelFormat::R16G16_UINT:
        case PixelFormat::R16G16_SINT:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R16G16_FLOAT:
            return D3DFMT_G16R16F;

        case PixelFormat::R32G32_UINT:
        case PixelFormat::R32G32_SINT:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R32G32_FLOAT:
            return D3DFMT_G32R32F;

        case PixelFormat::R4G4B4X4_UNORM:
            return D3DFMT_X4R4G4B4;
        case PixelFormat::R5G5B5X1_UNORM:
            return D3DFMT_X1R5G5B5;
        case PixelFormat::R5G6B5_UNORM:
            return D3DFMT_R5G6B5;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R8G8B8X8_UNORM:
            {
                constexpr D3DFORMAT r8g8b8x8_map[4] = {
                    D3DFMT_X8B8G8R8, // RGBA
                    D3DFMT_X8R8G8B8, // ARGB
                    D3DFMT_X8B8G8R8, // ABGR
                    D3DFMT_X8R8G8B8, // BGRA
                };
                return r8g8b8x8_map[ static_cast<unsigned>( channel_order ) ];
            }

        case PixelFormat::R11G11B10_FLOAT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R32G32B32_UINT:
        case PixelFormat::R32G32B32_SINT:
        case PixelFormat::R32G32B32_FLOAT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R4G4B4A4_UNORM:
            return D3DFMT_A4R4G4B4;
        case PixelFormat::R5G5B5A1_UNORM:
            return D3DFMT_A1R5G5B5;

        case PixelFormat::R8G8B8A8_UNORM:
            {
                constexpr D3DFORMAT r8g8b8a8_map[4] = {
                    D3DFMT_A8B8G8R8, // RGBA
                    D3DFMT_A8R8G8B8, // ARGB
                    D3DFMT_A8B8G8R8, // ABGR
                    D3DFMT_A8R8G8B8, // BGRA
                };
                return r8g8b8a8_map[ static_cast<unsigned>( channel_order ) ];
            }
        case PixelFormat::R8G8B8A8_SNORM:
            return D3DFMT_Q8W8V8U8;
        case PixelFormat::R8G8B8A8_UINT:
        case PixelFormat::R8G8B8A8_SINT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R10G10B10A2_UNORM:
            {
                constexpr D3DFORMAT r10g10b10a2_map[4] = {
                    D3DFMT_A2B10G10R10, // RGBA
                    D3DFMT_A2R10G10B10, // ARGB
                    D3DFMT_A2B10G10R10, // ABGR
                    D3DFMT_A2R10G10B10, // BGRA
                };
                return r10g10b10a2_map[ static_cast<unsigned>( channel_order ) ];
            }
        case PixelFormat::R10G10B10A2_UINT:
            return D3DFMT_UNKNOWN;

        case PixelFormat::R16G16B16A16_UNORM:
            return D3DFMT_A16B16G16R16;
        case PixelFormat::R16G16B16A16_SNORM:
            return D3DFMT_Q16W16V16U16;
        case PixelFormat::R16G16B16A16_UINT:
        case PixelFormat::R16G16B16A16_SINT:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R16G16B16A16_FLOAT:
            return D3DFMT_A16B16G16R16F;

        case PixelFormat::R32G32B32A32_UINT:
        case PixelFormat::R32G32B32A32_SINT:
            return D3DFMT_UNKNOWN;
        case PixelFormat::R32G32B32A32_FLOAT:
            return D3DFMT_A32B32G32R32F;

        case PixelFormat::A8_UNORM:
            return D3DFMT_A8;

        default:
            assert( false );
        }

        return D3DFMT_UNKNOWN;
    }

    std::pair<PixelFormat, ChannelOrder> _map_d3d_format(D3DFORMAT d3d_format, ChannelOrder preferred_channel_order)
    {
        auto rgb = [](ChannelOrder preferred_channel_order) -> ChannelOrder {
            return preferred_channel_order ==  ChannelOrder::ARGB ?  ChannelOrder::ARGB :  ChannelOrder::RGBA;
        };
        auto bgr = [](ChannelOrder preferred_channel_order) -> ChannelOrder {
            return preferred_channel_order ==  ChannelOrder::BGRA ?  ChannelOrder::BGRA :  ChannelOrder::ABGR;
        };

        switch ( d3d_format )
        {
        case D3DFMT_L8:
            return { PixelFormat::R8_UNORM, preferred_channel_order };

        case D3DFMT_L16:
            return { PixelFormat::R16_UNORM, preferred_channel_order };

        case D3DFMT_R32F:
            return { PixelFormat::R32_FLOAT, preferred_channel_order };

        case D3DFMT_V8U8:
            return { PixelFormat::R8G8_SNORM, rgb( preferred_channel_order ) };

        case D3DFMT_G16R16:
            return { PixelFormat::R16G16_UNORM, rgb( preferred_channel_order ) };
        case D3DFMT_V16U16:
            return { PixelFormat::R16G16_SNORM, rgb( preferred_channel_order ) };
        case D3DFMT_G16R16F:
            return { PixelFormat::R16G16_FLOAT, rgb( preferred_channel_order ) };

        case D3DFMT_G32R32F:
            return { PixelFormat::R32G32_FLOAT, rgb( preferred_channel_order ) };

        case D3DFMT_X4R4G4B4:
            return { PixelFormat::R4G4B4X4_UNORM, ChannelOrder::BGRA };
        case D3DFMT_X1R5G5B5:
            return { PixelFormat::R5G5B5X1_UNORM, ChannelOrder::BGRA };
        case D3DFMT_R5G6B5:
            return { PixelFormat::R5G6B5_UNORM, bgr( preferred_channel_order ) };

        case D3DFMT_X8B8G8R8:
            return { PixelFormat::R8G8B8X8_UNORM, ChannelOrder::RGBA };
        case D3DFMT_X8R8G8B8:
            return { PixelFormat::R8G8B8X8_UNORM, ChannelOrder::BGRA };

        case D3DFMT_A4R4G4B4:
            return { PixelFormat::R4G4B4A4_UNORM, ChannelOrder::BGRA };
        case D3DFMT_A1R5G5B5:
            return { PixelFormat::R5G5B5A1_UNORM, ChannelOrder::BGRA };

        case D3DFMT_A8B8G8R8:
            return { PixelFormat::R8G8B8A8_UNORM, ChannelOrder::RGBA };
        case D3DFMT_A8R8G8B8:
            return { PixelFormat::R8G8B8A8_UNORM, ChannelOrder::BGRA };
        case D3DFMT_Q8W8V8U8:
            return { PixelFormat::R8G8B8A8_SNORM, ChannelOrder::RGBA };

        case D3DFMT_A2B10G10R10:
            return { PixelFormat::R10G10B10A2_UNORM, ChannelOrder::RGBA };
        case D3DFMT_A2R10G10B10:
            return { PixelFormat::R10G10B10A2_UNORM, ChannelOrder::BGRA };

        case D3DFMT_A16B16G16R16:
            return { PixelFormat::R16G16B16A16_UNORM, ChannelOrder::RGBA };
        case D3DFMT_Q16W16V16U16:
            return { PixelFormat::R16G16B16A16_SNORM, ChannelOrder::RGBA };
        case D3DFMT_A16B16G16R16F:
            return { PixelFormat::R16G16B16A16_FLOAT, ChannelOrder::RGBA };

        case D3DFMT_A32B32G32R32F:
            return { PixelFormat::R32G32B32A32_FLOAT, ChannelOrder::RGBA };

        case D3DFMT_A8:
            return { PixelFormat::A8_UNORM, preferred_channel_order };
        }

        return { PixelFormat::UNKNOWN, preferred_channel_order };
    }

    D3DCMPFUNC _d3d_cmp_func_from(ComparisonFunction comparison_function)
    {
        static_assert( static_cast<unsigned>( ComparisonFunction::Never ) == static_cast<unsigned>( D3DCMP_NEVER ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::Less ) == static_cast<unsigned>( D3DCMP_LESS ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::Equal ) == static_cast<unsigned>( D3DCMP_EQUAL ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::LessEqual ) == static_cast<unsigned>( D3DCMP_LESSEQUAL ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::Greater ) == static_cast<unsigned>( D3DCMP_GREATER ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::NotEqual ) == static_cast<unsigned>( D3DCMP_NOTEQUAL ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::GreaterEqual ) == static_cast<unsigned>( D3DCMP_GREATEREQUAL ) );
        static_assert( static_cast<unsigned>( ComparisonFunction::Always ) == static_cast<unsigned>( D3DCMP_ALWAYS ) );

        return static_cast<D3DCMPFUNC>( comparison_function );
    }



} // namespace mappings
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
