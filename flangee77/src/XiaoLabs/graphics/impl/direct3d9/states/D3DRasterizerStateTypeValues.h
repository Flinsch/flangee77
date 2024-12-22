#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H

#include "../prerequisites.h"

#include <array>



namespace xl7::graphics::impl::direct3d9::states {



static constexpr size_t D3D_RASTERIZER_STATE_TYPE_COUNT = 2;
using D3DRasterizerStateTypeValues = std::array<std::pair<D3DRENDERSTATETYPE, DWORD>, D3D_RASTERIZER_STATE_TYPE_COUNT>;



} // namespace xl7::graphics::impl::direct3d9::states

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H
