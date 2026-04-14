#include "ConstantBuffer.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::shaders {



    ConstantBuffer::ConstantBuffer(const CreateContext& ctx, const ConstantBufferDesc& desc)
        : UpdatableResource(ctx, false, desc.layout.calculate_size())
        , _desc(desc)
    {
        const size_t data_size = desc.layout.calculate_size();
        assert(data_size > 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.buffers.max_constant_buffer_size && data_size > capabilities.buffers.max_constant_buffer_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a size of " + cl7::to_string(data_size) + u8" bytes, but a maximum of " + cl7::to_string(capabilities.buffers.max_constant_buffer_size) + u8" bytes is supported.");
    }



} // namespace xl7::graphics::shaders
