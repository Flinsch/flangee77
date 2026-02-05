#include "Texture2D.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <MathLabs/functions.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    /**
     * Updates the contents of this texture (unless it is immutable).
     */
    bool Texture2D::update(const ImageDataProvider& image_data_provider)
    {
        return _update(image_data_provider);
    }



    Texture2D::Texture2D(const CreateParams<Desc>& params)
        : TextureBase(Type::Texture2D, {.manager = params.manager, .id = params.id, .identifier = params.identifier, .desc = params.desc}, 1, 1) // NOLINT(*-slicing)
        , _desc(params.desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_2d_width && _desc.width > capabilities.textures.max_texture_2d_width)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_2d_width) + u8" is supported.");
        if (capabilities.textures.max_texture_2d_height && _desc.height > capabilities.textures.max_texture_2d_height)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_2d_height) + u8" is supported.");

        if (capabilities.textures.texture_2d_pow2_only && !ml7::is_power_of_two(_desc.width))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_2d_pow2_only && !ml7::is_power_of_two(_desc.height))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but only dimensions specified as a power of two are supported.");
    }



} // namespace xl7::graphics::textures
