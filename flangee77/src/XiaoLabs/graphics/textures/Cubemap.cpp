#include "Cubemap.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <MathLabs/functions.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Cubemap::Cubemap(const CreateContext& ctx, const CubemapDesc& desc)
        : ResourceBaseDirty(Type::Cubemap, ctx, {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
        }, desc.width, desc.height, 1, 6)
        , _desc(desc)
    {
        assert(static_cast<const void*>(&Texture::get_desc()) != static_cast<const void*>(&get_desc()));
        assert(::memcmp(&Texture::get_desc(), &get_desc(), sizeof(TextureDesc)) == 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_cubemap_size && _desc.width > capabilities.textures.max_cubemap_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_cubemap_size) + u8" is supported.");
        if (capabilities.textures.max_cubemap_size && _desc.height > capabilities.textures.max_cubemap_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_cubemap_size) + u8" is supported.");

        if (capabilities.textures.cubemap_pow2_only && !ml7::is_power_of_two(_desc.width))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.cubemap_pow2_only && !ml7::is_power_of_two(_desc.height))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but only dimensions specified as a power of two are supported.");

    }



} // namespace xl7::graphics::textures
