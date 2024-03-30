#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H

#include "../prerequisites.h"

#include <array>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



static constexpr size_t D3D_RASTERIZER_STATE_TYPE_COUNT = 2;
typedef std::array<std::pair<D3DRENDERSTATETYPE, DWORD>, D3D_RASTERIZER_STATE_TYPE_COUNT> D3DRasterizerStateTypeValues;



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_D3DRASTERIZERSTATETYPEVALUES_H
