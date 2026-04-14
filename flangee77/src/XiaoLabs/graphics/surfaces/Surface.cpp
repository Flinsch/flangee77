#include "Surface.h"



namespace xl7::graphics::surfaces {



    Surface::Surface(const CreateContext& ctx, Type type, const SurfaceDesc& desc)
        : ResourceBase(ctx)
        , _type(type)
        , _desc(desc)
    {
        assert(desc.width > 0 && desc.height > 0);
    }



} // namespace xl7::graphics::surfaces
