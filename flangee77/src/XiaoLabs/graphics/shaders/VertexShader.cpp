#include "VertexShader.h"



namespace xl7::graphics::shaders {



    VertexShader::VertexShader(const CreateContext& ctx, const ShaderDesc& desc)
        : ResourceBase(ctx, Type::VertexShader, desc)
    {
    }



} // namespace xl7::graphics::shaders
