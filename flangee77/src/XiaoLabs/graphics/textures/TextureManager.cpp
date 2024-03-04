#include "TextureManager.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    TextureManager::TextureManager(ITextureFactory* factory)
        : ResourceManager()
        , _factory( factory )
    {
    }

    /**
     * Destructor.
     */
    TextureManager::~TextureManager()
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Creates and acquires the specified 2D texture.
     */
    resources::ResourceID TextureManager::create_texture_2d(cl7::string_view identifier, const Texture2D::Desc& desc, const ImageDataProvider& data_provider)
    {
        resources::Resource::CreateParams<Texture2D::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr vertex_buffer( _factory->create_texture_2d( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(vertex_buffer), data_provider );
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
