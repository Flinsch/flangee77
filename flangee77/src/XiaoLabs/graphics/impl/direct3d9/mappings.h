#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H
#define XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H

#include "../../../resources/ResourceUsage.h"

#include "../../PixelFormat.h"
#include "../../ChannelOrder.h"

#include "../../ComparisonFunction.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace mappings {



    DWORD _d3d_usage_from(resources::ResourceUsage resource_usage);
    DWORD _d3d_usage_from(resources::ResourceUsage resource_usage, unsigned mip_levels);

    D3DPOOL _d3d_pool_from(resources::ResourceUsage resource_usage);

    D3DFORMAT _d3d_format_from(PixelFormat pixel_format, ChannelOrder channel_order);

    std::pair<PixelFormat, ChannelOrder> _map_d3d_format(D3DFORMAT d3d_format, ChannelOrder preferred_channel_order);

    D3DCMPFUNC _d3d_cmp_func_from(ComparisonFunction comparison_function);



} // namespace mappings
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H
