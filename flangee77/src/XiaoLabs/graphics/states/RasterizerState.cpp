#include "RasterizerState.h"



namespace xl7::graphics::states {



    RasterizerState::RasterizerState(const CreateContext& ctx, const RasterizerStateDesc& desc)
        : ResourceBase(ctx)
        , _desc(desc)
    {
    }



} // namespace xl7::graphics::states
