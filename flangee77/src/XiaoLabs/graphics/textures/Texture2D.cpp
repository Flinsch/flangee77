#include "Texture2D.h"



namespace xl7::graphics::textures {



    /**
     * Updates the contents of this texture (unless it is immutable).
     */
    bool Texture2D::update(const ImageDataProvider& image_data_provider)
    {
        return _update(image_data_provider);
    }



    Texture2D::Texture2D(const CreateParams<Desc>& params)
        : Texture(Type::Texture2D, {.manager = params.manager, .id = params.id, .identifier = params.identifier, .desc = params.desc}, 1, 1)
        , _desc(params.desc)
    {
    }



} // namespace xl7::graphics::textures
