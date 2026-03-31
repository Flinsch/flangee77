#include "SamplerState.h"



namespace xl7::graphics::states {



    SamplerState::SamplerState(const CreateContext& ctx, const SamplerStateDesc& desc)
        : ResourceBase(ctx)
        , _desc(desc)
    {
    }



} // namespace xl7::graphics::states
