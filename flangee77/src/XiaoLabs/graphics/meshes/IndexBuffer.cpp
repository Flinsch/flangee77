#include "IndexBuffer.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include "./MeshUtil.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    IndexBuffer::IndexBuffer(const CreateContext& ctx, const IndexBufferDesc& desc)
        : ResourceBase(ctx, Type::IndexBuffer, MeshBufferDesc{
            .usage = desc.usage,
            .topology = desc.topology,
            .element_count = desc.index_count,
            .element_stride = MeshUtil::get_index_stride(desc.index_type),
        })
        , _desc(desc)
    {
        assert(get_element_stride() == 2 || get_element_stride() == 4);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.buffers.max_index_buffer_size && get_data_size() > capabilities.buffers.max_index_buffer_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a size of " + cl7::to_string(get_data_size()) + u8" bytes, but a maximum of " + cl7::to_string(capabilities.buffers.max_index_buffer_size) + u8" bytes is supported.");
    }



} // namespace xl7::graphics::meshes
