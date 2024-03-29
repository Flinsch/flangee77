#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H

#include "../prerequisites.h"

#include <array>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



static constexpr size_t D3D_SAMPLER_STATE_TYPE_COUNT = 13;
typedef std::array<std::pair<D3DSAMPLERSTATETYPE, DWORD>, D3D_SAMPLER_STATE_TYPE_COUNT> D3DSamplerStateTypeValues;



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H
