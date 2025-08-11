#include "Texture3D.h"



namespace xl7::graphics::textures {



    Texture3D::Texture3D(const CreateParams<Desc>& params)
        : Texture(Type::Texture3D, {.manager = params.manager, .id = params.id, .identifier = params.identifier, .desc = params.desc}, params.desc.depth, 1) // NOLINT(*-slicing)
        , _desc(params.desc)
    {
    }



} // namespace xl7::graphics::textures
