#include "TextureManager.h"



namespace xl7::graphics::textures {



    /**
     * Creates and acquires the specified 2D texture.
     */
    Texture2D::Id TextureManager::create_texture_2d(cl7::u8string_view identifier, const Texture2DDesc& desc, const Texture2DWrite* initial_write)
    {
        resources::Resource::CreateContext ctx{.manager = this, .id = _next_id(), .identifier = identifier};

        auto* texture_2d = _factory->create_texture_2d(ctx, desc);
        ResourcePtr resource_ptr(texture_2d, _destroy_resource);

        if (initial_write)
            texture_2d->edit().write(*initial_write);
        else
            texture_2d->check_is_mutable();

        return _try_acquire_and_add_resource<Texture2D::Id>(std::move(resource_ptr));
    }



} // namespace xl7::graphics::textures
