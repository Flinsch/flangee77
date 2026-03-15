#include "Texture3D.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <MathLabs/functions.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture3D::Texture3D(const CreateContext& ctx, const Texture3DDesc& desc)
        : ResourceBaseDirty(Type::Texture3D, ctx, {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
        }, desc.width, desc.height, desc.depth, 1)
        , _desc(desc)
    {
        assert(static_cast<const void*>(&Texture::get_desc()) != static_cast<const void*>(&get_desc()));
        assert(::memcmp(&Texture::get_desc(), &get_desc(), sizeof(TextureDesc)) == 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_3d_size && _desc.width > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");
        if (capabilities.textures.max_texture_3d_size && _desc.height > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");
        if (capabilities.textures.max_texture_3d_size && desc.depth > capabilities.textures.max_texture_3d_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a depth of " + cl7::to_string(desc.depth) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_texture_3d_size) + u8" is supported.");

        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(_desc.width))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a width of " + cl7::to_string(_desc.width) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(_desc.height))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a height of " + cl7::to_string(_desc.height) + u8", but only dimensions specified as a power of two are supported.");
        if (capabilities.textures.texture_3d_pow2_only && !ml7::is_power_of_two(desc.depth))
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a depth of " + cl7::to_string(desc.depth) + u8", but only dimensions specified as a power of two are supported.");
    }



} // namespace xl7::graphics::textures
