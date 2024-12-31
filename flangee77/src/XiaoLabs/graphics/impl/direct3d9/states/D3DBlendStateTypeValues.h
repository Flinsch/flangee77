#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_D3DBLENDSTATETYPEVALUES_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_D3DBLENDSTATETYPEVALUES_H

#include "../prerequisites.h"

#include <array>



namespace xl7::graphics::impl::direct3d9::states {



static constexpr size_t D3D_BLEND_STATE_TYPE_COUNT = 9;
using D3DBlendStateTypeValues = std::array<std::pair<D3DRENDERSTATETYPE, DWORD>, D3D_BLEND_STATE_TYPE_COUNT>;



} // namespace xl7::graphics::impl::direct3d9::states

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_D3DBLENDSTATETYPEVALUES_H
