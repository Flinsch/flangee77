#include "TextureManager.h"



namespace xl7::graphics::textures {



    /**
     * Creates and acquires the specified 2D texture.
     */
    Texture2D::ID TextureManager::create_texture_2d(cl7::u8string_view identifier, const Texture2D::Desc& desc, const ImageDataProvider& image_data_provider)
    {
        resources::Resource::CreateParams<Texture2D::Desc> params{.manager=this, .id=_next_id(), .identifier=identifier, .desc=desc};

        ResourcePtr vertex_buffer(_factory->create_texture_2d(params), _destroy_resource);

        return _try_acquire_and_add_resource<Texture2D::ID>(std::move(vertex_buffer), image_data_provider);
    }



} // namespace xl7::graphics::textures
