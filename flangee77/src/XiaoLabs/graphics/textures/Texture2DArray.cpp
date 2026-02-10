#include "Texture2DArray.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture2DArray::Texture2DArray(const CreateParams<Desc>& params)
        : ResourceBase(Type::Texture2DArray, {.manager = params.manager, .id = params.id, .identifier = params.identifier, .desc = params.desc}, 1, params.desc.count) // NOLINT(*-slicing)
        , _desc(params.desc)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.max_texture_array_size && params.desc.count > capabilities.textures.max_texture_array_size)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created should have " + cl7::to_string(params.desc.count) + u8" slices, but a maximum of " + cl7::to_string(capabilities.textures.max_texture_array_size) + u8" is supported.");
    }



} // namespace xl7::graphics::textures
