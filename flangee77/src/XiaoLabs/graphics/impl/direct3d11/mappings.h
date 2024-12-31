#ifndef XL7_GRAPHICS_IMPL_D3D11_MAPPINGS_H
#define XL7_GRAPHICS_IMPL_D3D11_MAPPINGS_H

#include "../../../resources/ResourceUsage.h"

#include "../../PixelFormat.h"
#include "../../ChannelOrder.h"

#include "../../ComparisonFunction.h"

#include "./prerequisites.h"



namespace xl7::graphics::impl::direct3d11::mappings {



    D3D11_USAGE _d3d_usage_from(resources::ResourceUsage resource_usage);

    DXGI_FORMAT _dxgi_format_from(PixelFormat pixel_format, ChannelOrder channel_order);

    std::pair<PixelFormat, ChannelOrder> _map_dxgi_format(DXGI_FORMAT d3d_format, ChannelOrder preferred_channel_order);

    D3D11_COMPARISON_FUNC _d3d_comparison_func_from(ComparisonFunction comparison_function);



} // namespace xl7::graphics::impl::direct3d11::mappings

#endif // XL7_GRAPHICS_IMPL_D3D11_MAPPINGS_H
