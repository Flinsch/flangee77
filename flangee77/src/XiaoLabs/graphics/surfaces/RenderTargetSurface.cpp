#include "RenderTargetSurface.h"



namespace xl7::graphics::surfaces {



    RenderTargetSurface::RenderTargetSurface(const CreateContext& ctx, const SurfaceDesc& desc)
        : ResourceBase(ctx, Type::RenderTargetSurface, desc)
    {
    }



} // namespace xl7::graphics::surfaces
