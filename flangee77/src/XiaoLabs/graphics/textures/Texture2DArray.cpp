#include "Texture2DArray.h"



namespace xl7::graphics::textures {



    Texture2DArray::Texture2DArray(const CreateParams<Desc>& params)
        : Texture(Type::Texture2DArray, {.manager=params.manager, .id=params.id, .identifier=params.identifier, .desc=params.desc}, 1, params.desc.count)
        , _desc(params.desc)
    {
    }



} // namespace xl7::graphics::textures
