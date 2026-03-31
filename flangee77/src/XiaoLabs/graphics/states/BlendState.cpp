#include "BlendState.h"



namespace xl7::graphics::states {



    BlendState::BlendState(const CreateContext& ctx, const BlendStateDesc& desc)
        : ResourceBase(ctx)
        , _desc(desc)
    {
    }



} // namespace xl7::graphics::states
