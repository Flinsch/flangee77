#include "FramebufferSurface.h"



namespace xl7::graphics::surfaces {



    FramebufferSurface::FramebufferSurface(const CreateContext& ctx, Type type, const SurfaceDesc& desc)
        : ResourceBase(ctx, type, desc)
        , _default_viewport{.x = 0, .y = 0, .width = desc.width, .height = desc.height, .min_z = 0.0f, .max_z = 1.0f}
    {
        assert(type == Type::RenderTargetSurface || type == Type::DepthStencilSurface);
    }



} // namespace xl7::graphics::surfaces
