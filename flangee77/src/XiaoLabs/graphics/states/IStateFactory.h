#ifndef XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H
#define XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H

#include "./SamplerState.h"
#include "./RasterizerState.h"
#include "./DepthStencilState.h"
#include "./BlendState.h"



namespace xl7::graphics::states {



class StateManager;



class IStateFactory
{
public:
    virtual ~IStateFactory() = default;

    virtual SamplerState* create_sampler_state(const resources::Resource::CreateContext& ctx, const SamplerStateDesc& desc) = 0;
    virtual RasterizerState* create_rasterizer_state(const resources::Resource::CreateContext& ctx, const RasterizerStateDesc& desc) = 0;
    virtual DepthStencilState* create_depth_stencil_state(const resources::Resource::CreateContext& ctx, const DepthStencilStateDesc& desc) = 0;
    virtual BlendState* create_blend_state(const resources::Resource::CreateContext& ctx, const BlendStateDesc& desc) = 0;

}; // class IStateFactory



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_SURFACES_ISTATEFACTORY_H
