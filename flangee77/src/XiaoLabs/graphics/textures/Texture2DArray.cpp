#include "Texture2DArray.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture2DArray::Texture2DArray(const CreateContext& ctx, const Texture2DArrayDesc& desc)
        : ResourceBase(ctx, Type::Texture2DArray, TextureDesc{
            .usage = desc.usage,
            .pixel_format = desc.pixel_format,
            .preferred_channel_order = desc.preferred_channel_order,
            .mip_levels = desc.mip_levels,
            .width = desc.width,
            .height = desc.height,
            .depth = 1,
            .layer_count = desc.layer_count,
        })
        , _desc(desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_array_size && desc.layer_count > capabilities.textures.max_texture_array_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created should have " + cl7::to_string(desc.layer_count) + u8" texture layers, but a maximum of " + cl7::to_string(capabilities.textures.max_texture_array_size) + u8" is supported.");
    }



} // namespace xl7::graphics::textures
