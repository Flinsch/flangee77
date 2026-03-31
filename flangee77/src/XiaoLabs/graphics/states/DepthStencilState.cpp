#include "DepthStencilState.h"



namespace xl7::graphics::states {



    DepthStencilState::DepthStencilState(const CreateContext& ctx, const DepthStencilStateDesc& desc)
        : ResourceBase(ctx)
        , _desc(desc)
    {
    }



} // namespace xl7::graphics::states
