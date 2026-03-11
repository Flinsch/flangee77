#include "Texture2DArray.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture2DArray::Texture2DArray(const CreateContext& ctx, const Texture2DArrayDesc& desc)
        : ResourceBaseDirty(Type::Texture2DArray, ctx, desc, 1, desc.count) // NOLINT(*-slicing)
        , _desc(desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_array_size && desc.count > capabilities.textures.max_texture_array_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created should have " + cl7::to_string(desc.count) + u8" layers, but a maximum of " + cl7::to_string(capabilities.textures.max_texture_array_size) + u8" is supported.");
    }



} // namespace xl7::graphics::textures
