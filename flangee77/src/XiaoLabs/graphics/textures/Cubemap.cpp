#include "Cubemap.h"



namespace xl7::graphics::textures {



    Cubemap::Cubemap(const CreateParams<Desc>& params)
        : Texture(Type::Cubemap, {.manager = params.manager, .id = params.id, .identifier = params.identifier, .desc = params.desc}, 1, 6)
        , _desc(params.desc)
    {
    }



} // namespace xl7::graphics::textures
