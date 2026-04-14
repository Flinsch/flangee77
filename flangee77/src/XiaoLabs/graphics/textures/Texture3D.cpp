#include "Texture3D.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <MathLabs/functions.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture3D::Texture3D(const CreateContext& ctx, const Texture3DDesc& desc)
        : ResourceBase(ctx, Type::Texture3D, TextureDesc{
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .extent = desc.extent.generalize(),
            .layer_count = 1,
        })
        , _desc(desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_3d_size && _desc.extent.width > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a width of " + cl7::to_string(_desc.extent.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");
        if (capabilities.textures.max_texture_3d_size && _desc.extent.height > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a height of " + cl7::to_string(_desc.extent.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");
        if (capabilities.textures.max_texture_3d_size && _desc.extent.depth > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a depth of " + cl7::to_string(_desc.extent.depth) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");

        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(_desc.extent.width))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a width of " + cl7::to_string(_desc.extent.width) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(_desc.extent.height))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a height of " + cl7::to_string(_desc.extent.height) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(_desc.extent.depth))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a depth of " + cl7::to_string(_desc.extent.depth) + u8", but only dimensions specified as a power of two are supported.");
    }



} // namespace xl7::graphics::textures
