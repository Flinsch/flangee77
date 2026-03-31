#include "VertexBuffer.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    VertexBuffer::VertexBuffer(const CreateContext& ctx, const VertexBufferDesc& desc)
        : ResourceBase(ctx, Type::VertexBuffer, MeshBufferDesc{
            .usage = desc.usage,
            .topology = desc.topology,
            .element_count = desc.vertex_count,
            .element_stride = desc.vertex_stride,
        })
        , _desc(desc)
    {
        assert(get_element_stride() > 0 && get_element_stride() >= _desc.vertex_layout.calculate_size());
        assert(get_element_stride() >= 12 && get_element_stride() % 4 == 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.buffers.max_vertex_buffer_size && get_data_size() > capabilities.buffers.max_vertex_buffer_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a size of " + cl7::to_string(get_data_size()) + u8" bytes, but a maximum of " + cl7::to_string(capabilities.buffers.max_vertex_buffer_size) + u8" bytes is supported.");

        if (get_element_stride() < 16)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a stride of " + cl7::to_string(get_element_stride()) + u8" bytes. A vertex layout should preferably comprise at least 16 bytes to avoid issues due to hardware alignment/padding expectations.");
        if (get_element_stride() % 16 != 0)
            LOG_INFO(u8"The " + get_qualified_identifier() + u8" to be created has a stride of " + cl7::to_string(get_element_stride()) + u8" bytes. For optimal performance, align your vertex structure to (a multiple of) 16 bytes (or even 32 bytes for some modern GPUs).");
    }



} // namespace xl7::graphics::meshes
