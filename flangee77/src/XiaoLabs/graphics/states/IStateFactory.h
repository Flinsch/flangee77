#pragma once
#ifndef XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H
#define XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H

#include "./SamplerState.h"
#include "./RasterizerState.h"
#include "./DepthStencilState.h"
#include "./BlendState.h"



namespace xl7 {
namespace graphics {
namespace states {



class StateManager;



class IStateFactory
{
public:
    virtual ~IStateFactory() = default;

    virtual xl7::graphics::states::SamplerState* create_sampler_state(const resources::Resource::CreateParams<xl7::graphics::states::SamplerState::Desc>& params) = 0;
    virtual xl7::graphics::states::RasterizerState* create_rasterizer_state(const resources::Resource::CreateParams<xl7::graphics::states::RasterizerState::Desc>& params) = 0;
    virtual xl7::graphics::states::DepthStencilState* create_depth_stencil_state(const resources::Resource::CreateParams<xl7::graphics::states::DepthStencilState::Desc>& params) = 0;
    virtual xl7::graphics::states::BlendState* create_blend_state(const resources::Resource::CreateParams<xl7::graphics::states::BlendState::Desc>& params) = 0;

}; // class IStateFactory



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H
