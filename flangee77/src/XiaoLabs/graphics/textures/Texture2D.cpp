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



    Texture2D::Texture2D(const CreateContext& ctx, const Texture2DDesc& desc)
        : ResourceBase(ctx, Type::Texture2D, TextureDesc{
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .layer_count = 1,
        })
        , _desc(desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_2d_width && _desc.width > capabilities.textures.max_texture_2d_width)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_2d_width) + u8" is supported.");
        if (capabilities.textures.max_texture_2d_height && _desc.height > capabilities.textures.max_texture_2d_height)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_2d_height) + u8" is supported.");

        if (capabilities.textures.texture_2d_pow2_only && !ml7::is_power_of_two(_desc.width))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_2d_pow2_only && !ml7::is_power_of_two(_desc.height))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but only dimensions specified as a power of two are supported.");
    }



} // namespace xl7::graphics::textures
