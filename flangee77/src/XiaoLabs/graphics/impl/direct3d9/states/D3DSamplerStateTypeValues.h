#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H

#include "../prerequisites.h"

#include <array>



namespace xl7::graphics::impl::direct3d9::states {



static constexpr size_t D3D_SAMPLER_STATE_TYPE_COUNT = 13;
using D3DSamplerStateTypeValues = std::array<std::pair<D3DSAMPLERSTATETYPE, DWORD>, D3D_SAMPLER_STATE_TYPE_COUNT>;



} // namespace xl7::graphics::impl::direct3d9::states

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_D3DSAMPLERSTATETYPEVALUES_H
