#include "mappings.h"



namespace xl7::graphics::impl::direct3d11::mappings {



    D3D11_USAGE _d3d_usage_from(graphics::meshes::MeshBufferUsage mesh_buffer_usage)
    {
        switch (mesh_buffer_usage)
        {
        case graphics::meshes::MeshBufferUsage::Immutable:
            return D3D11_USAGE_IMMUTABLE;
        case graphics::meshes::MeshBufferUsage::Default:
            return D3D11_USAGE_DEFAULT;
        case graphics::meshes::MeshBufferUsage::Dynamic:
        case graphics::meshes::MeshBufferUsage::Transient:
            return D3D11_USAGE_DYNAMIC;
        default:
            assert(false);
            return D3D11_USAGE_DEFAULT;
        }
    }

    D3D11_USAGE _d3d_usage_from(graphics::textures::TextureUsage texture_usage)
    {
        switch (texture_usage)
        {
        case graphics::textures::TextureUsage::Immutable:
            return D3D11_USAGE_IMMUTABLE;
        case graphics::textures::TextureUsage::Default:
            return D3D11_USAGE_DEFAULT;
        case graphics::textures::TextureUsage::Dynamic:
            return D3D11_USAGE_DYNAMIC;
        default:
            assert(false);
            return D3D11_USAGE_DEFAULT;
        }
    }

    DXGI_FORMAT _dxgi_format_from(PixelFormat pixel_format, ChannelOrder channel_order)
    {
        static_assert(static_cast<unsigned>(ChannelOrder::RGBA) == 0);
        static_assert(static_cast<unsigned>(ChannelOrder::ARGB) == 1);
        static_assert(static_cast<unsigned>(ChannelOrder::ABGR) == 2);
        static_assert(static_cast<unsigned>(ChannelOrder::BGRA) == 3);

        switch (pixel_format)
        {
        case PixelFormat::UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;

        case PixelFormat::R8_UNORM:
            return DXGI_FORMAT_R8_UNORM;
        case PixelFormat::R8_SNORM:
            return DXGI_FORMAT_R8_SNORM;
        case PixelFormat::R8_UINT:
            return DXGI_FORMAT_R8_UINT;
        case PixelFormat::R8_SINT:
            return DXGI_FORMAT_R8_SINT;

        case PixelFormat::R16_UNORM:
            return DXGI_FORMAT_R16_UNORM;
        case PixelFormat::R16_SNORM:
            return DXGI_FORMAT_R16_SNORM;
        case PixelFormat::R16_UINT:
            return DXGI_FORMAT_R16_UINT;
        case PixelFormat::R16_SINT:
            return DXGI_FORMAT_R16_SINT;
        case PixelFormat::R16_FLOAT:
            return DXGI_FORMAT_R16_FLOAT;

        case PixelFormat::R32_UINT:
            return DXGI_FORMAT_R32_UINT;
        case PixelFormat::R32_SINT:
            return DXGI_FORMAT_R32_SINT;
        case PixelFormat::R32_FLOAT:
            return DXGI_FORMAT_R32_FLOAT;

        case PixelFormat::R8G8_UNORM:
            return DXGI_FORMAT_R8G8_UNORM;
        case PixelFormat::R8G8_SNORM:
            return DXGI_FORMAT_R8G8_SNORM;
        case PixelFormat::R8G8_UINT:
            return DXGI_FORMAT_R8G8_UINT;
        case PixelFormat::R8G8_SINT:
            return DXGI_FORMAT_R8G8_SINT;

        case PixelFormat::R16G16_UNORM:
            return DXGI_FORMAT_R16G16_UNORM;
        case PixelFormat::R16G16_SNORM:
            return DXGI_FORMAT_R16G16_SNORM;
        case PixelFormat::R16G16_UINT:
            return DXGI_FORMAT_R16G16_UINT;
        case PixelFormat::R16G16_SINT:
            return DXGI_FORMAT_R16G16_SINT;
        case PixelFormat::R16G16_FLOAT:
            return DXGI_FORMAT_R16G16_FLOAT;

        case PixelFormat::R32G32_UINT:
            return DXGI_FORMAT_R32G32_UINT;
        case PixelFormat::R32G32_SINT:
            return DXGI_FORMAT_R32G32_SINT;
        case PixelFormat::R32G32_FLOAT:
            return DXGI_FORMAT_R32G32_FLOAT;

        case PixelFormat::R4G4B4X4_UNORM:
            // Should we return DXGI_FORMAT_B4G4R4A4_UNORM here?
            return DXGI_FORMAT_UNKNOWN;
        case PixelFormat::R5G5B5X1_UNORM:
            // Should we return DXGI_FORMAT_B5G5R5A1_UNORM here?
            return DXGI_FORMAT_UNKNOWN;
        case PixelFormat::R5G6B5_UNORM:
            return DXGI_FORMAT_B5G6R5_UNORM;

        case PixelFormat::R8G8B8_UNORM:
        case PixelFormat::R8G8B8_SNORM:
        case PixelFormat::R8G8B8_UINT:
        case PixelFormat::R8G8B8_SINT:
            return DXGI_FORMAT_UNKNOWN;

        case PixelFormat::R8G8B8X8_UNORM:
            return DXGI_FORMAT_B8G8R8X8_UNORM;

        case PixelFormat::R11G11B10_FLOAT:
            return DXGI_FORMAT_R11G11B10_FLOAT;

        case PixelFormat::R16G16B16_UNORM:
        case PixelFormat::R16G16B16_FLOAT:
            return DXGI_FORMAT_UNKNOWN;

        case PixelFormat::R32G32B32_UINT:
            return DXGI_FORMAT_R32G32B32_UINT;
        case PixelFormat::R32G32B32_SINT:
            return DXGI_FORMAT_R32G32B32_SINT;
        case PixelFormat::R32G32B32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;

        case PixelFormat::R4G4B4A4_UNORM:
            return DXGI_FORMAT_B4G4R4A4_UNORM;
        case PixelFormat::R5G5B5A1_UNORM:
            return DXGI_FORMAT_B5G5R5A1_UNORM;

        case PixelFormat::R8G8B8A8_UNORM:
            {
                constexpr DXGI_FORMAT r8g8b8a8_map[4] = {
                    DXGI_FORMAT_R8G8B8A8_UNORM, // RGBA
                    DXGI_FORMAT_B8G8R8A8_UNORM, // ARGB
                    DXGI_FORMAT_R8G8B8A8_UNORM, // ABGR
                    DXGI_FORMAT_B8G8R8A8_UNORM, // BGRA
                };
                return r8g8b8a8_map[static_cast<unsigned>(channel_order)];
            }
        case PixelFormat::R8G8B8A8_SNORM:
            return DXGI_FORMAT_R8G8B8A8_SNORM;
        case PixelFormat::R8G8B8A8_UINT:
            return DXGI_FORMAT_R8G8B8A8_UINT;
        case PixelFormat::R8G8B8A8_SINT:
            return DXGI_FORMAT_R8G8B8A8_SINT;

        case PixelFormat::R10G10B10A2_UNORM:
            return DXGI_FORMAT_R10G10B10A2_UNORM;
        case PixelFormat::R10G10B10A2_UINT:
            return DXGI_FORMAT_R10G10B10A2_UINT;

        case PixelFormat::R16G16B16A16_UNORM:
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        case PixelFormat::R16G16B16A16_SNORM:
            return DXGI_FORMAT_R16G16B16A16_SNORM;
        case PixelFormat::R16G16B16A16_UINT:
            return DXGI_FORMAT_R16G16B16A16_UINT;
        case PixelFormat::R16G16B16A16_SINT:
            return DXGI_FORMAT_R16G16B16A16_SINT;
        case PixelFormat::R16G16B16A16_FLOAT:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;

        case PixelFormat::R32G32B32A32_UINT:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case PixelFormat::R32G32B32A32_SINT:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case PixelFormat::R32G32B32A32_FLOAT:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        case PixelFormat::A8_UNORM:
            return DXGI_FORMAT_A8_UNORM;

        default:
            assert(false);
            return DXGI_FORMAT_UNKNOWN;
        }
    }

    std::pair<PixelFormat, ChannelOrder> _map_dxgi_format(DXGI_FORMAT d3d_format, ChannelOrder preferred_channel_order)
    {
        auto rgb = [](ChannelOrder preferred_channel_order) -> ChannelOrder {
            return preferred_channel_order == ChannelOrder::ARGB ? ChannelOrder::ARGB : ChannelOrder::RGBA;
        };
        auto bgr = [](ChannelOrder preferred_channel_order) -> ChannelOrder {
            return preferred_channel_order == ChannelOrder::ABGR ? ChannelOrder::ABGR : ChannelOrder::BGRA;
        };

        switch (d3d_format)
        {
        case DXGI_FORMAT_R8_UNORM:
            return {PixelFormat::R8_UNORM, preferred_channel_order};
        case DXGI_FORMAT_R8_SNORM:
            return {PixelFormat::R8_SNORM, preferred_channel_order};
        case DXGI_FORMAT_R8_UINT:
            return {PixelFormat::R8_UINT, preferred_channel_order};
        case DXGI_FORMAT_R8_SINT:
            return {PixelFormat::R8_SINT, preferred_channel_order};

        case DXGI_FORMAT_R16_UNORM:
            return {PixelFormat::R16_UNORM, preferred_channel_order};
        case DXGI_FORMAT_R16_SNORM:
            return {PixelFormat::R16_SNORM, preferred_channel_order};
        case DXGI_FORMAT_R16_UINT:
            return {PixelFormat::R16_UINT, preferred_channel_order};
        case DXGI_FORMAT_R16_SINT:
            return {PixelFormat::R16_SINT, preferred_channel_order};
        case DXGI_FORMAT_R16_FLOAT:
            return {PixelFormat::R16_FLOAT, preferred_channel_order};

        case DXGI_FORMAT_R32_UINT:
            return {PixelFormat::R32_UINT, preferred_channel_order};
        case DXGI_FORMAT_R32_SINT:
            return {PixelFormat::R32_SINT, preferred_channel_order};
        case DXGI_FORMAT_R32_FLOAT:
            return {PixelFormat::R32_FLOAT, preferred_channel_order};

        case DXGI_FORMAT_R8G8_UNORM:
            return {PixelFormat::R8G8_UNORM, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R8G8_SNORM:
            return {PixelFormat::R8G8_SNORM, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R8G8_UINT:
            return {PixelFormat::R8G8_UINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R8G8_SINT:
            return {PixelFormat::R8G8_SINT, rgb(preferred_channel_order)};

        case DXGI_FORMAT_R16G16_UNORM:
            return {PixelFormat::R16G16_UNORM, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R16G16_SNORM:
            return {PixelFormat::R16G16_SNORM, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R16G16_UINT:
            return {PixelFormat::R16G16_UINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R16G16_SINT:
            return {PixelFormat::R16G16_SINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R16G16_FLOAT:
            return {PixelFormat::R16G16_FLOAT, rgb(preferred_channel_order)};

        case DXGI_FORMAT_R32G32_UINT:
            return {PixelFormat::R32G32_UINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R32G32_SINT:
            return {PixelFormat::R32G32_SINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R32G32_FLOAT:
            return {PixelFormat::R32G32_FLOAT, rgb(preferred_channel_order)};

        case DXGI_FORMAT_B5G6R5_UNORM:
            return {PixelFormat::R5G6B5_UNORM, bgr(preferred_channel_order)};

        case DXGI_FORMAT_B8G8R8X8_UNORM:
            return {PixelFormat::R8G8B8X8_UNORM, ChannelOrder::BGRA};

        case DXGI_FORMAT_R11G11B10_FLOAT:
            return {PixelFormat::R11G11B10_FLOAT, rgb(preferred_channel_order)};

        case DXGI_FORMAT_R32G32B32_UINT:
            return {PixelFormat::R32G32B32_UINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R32G32B32_SINT:
            return {PixelFormat::R32G32B32_SINT, rgb(preferred_channel_order)};
        case DXGI_FORMAT_R32G32B32_FLOAT:
            return {PixelFormat::R32G32B32_FLOAT, rgb(preferred_channel_order)};

        case DXGI_FORMAT_B4G4R4A4_UNORM:
            return {PixelFormat::R4G4B4A4_UNORM, ChannelOrder::BGRA};
        case DXGI_FORMAT_B5G5R5A1_UNORM:
            return {PixelFormat::R5G5B5A1_UNORM, ChannelOrder::BGRA};

        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return {PixelFormat::R8G8B8A8_UNORM, ChannelOrder::RGBA};
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return {PixelFormat::R8G8B8A8_UNORM, ChannelOrder::BGRA};
        case DXGI_FORMAT_R8G8B8A8_SNORM:
            return {PixelFormat::R8G8B8A8_SNORM, ChannelOrder::RGBA};
        case DXGI_FORMAT_R8G8B8A8_UINT:
            return {PixelFormat::R8G8B8A8_UINT, ChannelOrder::RGBA};
        case DXGI_FORMAT_R8G8B8A8_SINT:
            return {PixelFormat::R8G8B8A8_SINT, ChannelOrder::RGBA};

        case DXGI_FORMAT_R10G10B10A2_UNORM:
            return {PixelFormat::R10G10B10A2_UNORM, ChannelOrder::RGBA};
        case DXGI_FORMAT_R10G10B10A2_UINT:
            return {PixelFormat::R10G10B10A2_UINT, ChannelOrder::RGBA};

        case DXGI_FORMAT_R16G16B16A16_UNORM:
            return {PixelFormat::R16G16B16A16_UNORM, ChannelOrder::RGBA};
        case DXGI_FORMAT_R16G16B16A16_SNORM:
            return {PixelFormat::R16G16B16A16_SNORM , ChannelOrder::RGBA};
        case DXGI_FORMAT_R16G16B16A16_UINT:
            return {PixelFormat::R16G16B16A16_UINT, ChannelOrder::RGBA};
        case DXGI_FORMAT_R16G16B16A16_SINT:
            return {PixelFormat::R16G16B16A16_SINT, ChannelOrder::RGBA};
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            return {PixelFormat::R16G16B16A16_FLOAT, ChannelOrder::RGBA};

        case DXGI_FORMAT_R32G32B32A32_UINT:
            return {PixelFormat::R32G32B32A32_UINT, ChannelOrder::RGBA};
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return {PixelFormat::R32G32B32A32_SINT, ChannelOrder::RGBA};
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            return {PixelFormat::R32G32B32A32_FLOAT, ChannelOrder::RGBA};

        case DXGI_FORMAT_A8_UNORM:
            return {PixelFormat::A8_UNORM, preferred_channel_order};

        default:
            return {PixelFormat::UNKNOWN, preferred_channel_order};
        }
    }

    D3D11_COMPARISON_FUNC _d3d_comparison_func_from(ComparisonFunction comparison_function)
    {
        static_assert(static_cast<unsigned>(ComparisonFunction::Never) == static_cast<unsigned>(D3D11_COMPARISON_NEVER));
        static_assert(static_cast<unsigned>(ComparisonFunction::Less) == static_cast<unsigned>(D3D11_COMPARISON_LESS));
        static_assert(static_cast<unsigned>(ComparisonFunction::Equal) == static_cast<unsigned>(D3D11_COMPARISON_EQUAL));
        static_assert(static_cast<unsigned>(ComparisonFunction::LessEqual) == static_cast<unsigned>(D3D11_COMPARISON_LESS_EQUAL));
        static_assert(static_cast<unsigned>(ComparisonFunction::Greater) == static_cast<unsigned>(D3D11_COMPARISON_GREATER));
        static_assert(static_cast<unsigned>(ComparisonFunction::NotEqual) == static_cast<unsigned>(D3D11_COMPARISON_NOT_EQUAL));
        static_assert(static_cast<unsigned>(ComparisonFunction::GreaterEqual) == static_cast<unsigned>(D3D11_COMPARISON_GREATER_EQUAL));
        static_assert(static_cast<unsigned>(ComparisonFunction::Always) == static_cast<unsigned>(D3D11_COMPARISON_ALWAYS));

        return static_cast<D3D11_COMPARISON_FUNC>(comparison_function);
    }



} // namespace xl7::graphics::impl::direct3d11::mappings
