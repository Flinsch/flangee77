#include "VertexBuffer.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    VertexBuffer::VertexBuffer(const CreateParams<Desc>& params)
        : MeshBufferBase(Type::VertexBuffer, params, params.desc.stride)
        , _desc(params.desc)
    {
        assert(get_stride() > 0 && get_stride() >= _desc.vertex_layout.calculate_size());
        assert(get_stride() >= 12 && get_stride() % 4 == 0);

        if (get_stride() < 16)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has a stride of " + cl7::to_string(get_stride()) + u8" bytes. A vertex layout should preferably comprise at least 16 bytes to avoid issues due to hardware alignment/padding expectations.");
        if (get_stride() % 16 != 0)
            LOG_INFO(u8"The " + get_typed_identifier_string() + u8" to be created has a stride of " + cl7::to_string(get_stride()) + u8" bytes. For optimal performance, align your vertex structure to (a multiple of) 16 bytes (or even 32 bytes for some modern GPUs).");
    }



} // namespace xl7::graphics::meshes
