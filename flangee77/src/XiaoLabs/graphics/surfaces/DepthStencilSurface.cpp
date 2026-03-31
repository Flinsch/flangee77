#include "DepthStencilSurface.h"



namespace xl7::graphics::surfaces {



    DepthStencilSurface::DepthStencilSurface(const CreateContext& ctx, const SurfaceDesc& desc)
        : ResourceBase(ctx, Type::DepthStencilSurface, desc)
    {
    }



} // namespace xl7::graphics::surfaces
