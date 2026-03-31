#include "PixelShader.h"



namespace xl7::graphics::shaders {



    PixelShader::PixelShader(const CreateContext& ctx, const ShaderDesc& desc)
        : ResourceBase(ctx, Type::PixelShader, desc)
    {
    }



} // namespace xl7::graphics::shaders
