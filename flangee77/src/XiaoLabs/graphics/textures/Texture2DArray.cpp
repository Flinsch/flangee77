#include "Texture2DArray.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture2DArray::Texture2DArray(const CreateContext& ctx, const Texture2DArrayDesc& desc)
        : ResourceBaseDirty(Type::Texture2DArray, ctx, {
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
        }, desc.width, desc.height, 1, desc.texture_count)
        , _desc(desc)
    {
        assert(static_cast<const void*>(&Texture::get_desc()) != static_cast<const void*>(&get_desc()));
        assert(::memcmp(&Texture::get_desc(), &get_desc(), sizeof(TextureDesc)) == 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_array_size && desc.texture_count > capabilities.textures.max_texture_array_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created should have " + cl7::to_string(desc.texture_count) + u8" textures/layers, but a maximum of " + cl7::to_string(capabilities.textures.max_texture_array_size) + u8" is supported.");
    }



} // namespace xl7::graphics::textures
