#ifndef XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H
#define XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H

#include "../../meshes/MeshBufferUsage.h"
#include "../../textures/TextureUsage.h"

#include "../../PixelFormat.h"
#include "../../ChannelOrder.h"

#include "../../ComparisonFunction.h"

#include "./prerequisites.h"



namespace xl7::graphics::impl::direct3d9::mappings {



    DWORD _d3d_usage_from(graphics::meshes::MeshBufferUsage mesh_buffer_usage);
    DWORD _d3d_usage_from(graphics::textures::TextureUsage texture_usage, unsigned mip_levels);

    D3DPOOL _d3d_pool_from(graphics::meshes::MeshBufferUsage mesh_buffer_usage);
    D3DPOOL _d3d_pool_from(graphics::textures::TextureUsage texture_usage);

    D3DFORMAT _d3d_format_from(PixelFormat pixel_format, ChannelOrder channel_order);

    std::pair<PixelFormat, ChannelOrder> _map_d3d_format(D3DFORMAT d3d_format, ChannelOrder preferred_channel_order);

    D3DCMPFUNC _d3d_cmp_func_from(ComparisonFunction comparison_function);



} // namespace xl7::graphics::impl::direct3d9::mappings

#endif // XL7_GRAPHICS_IMPL_D3D9_MAPPINGS_H
