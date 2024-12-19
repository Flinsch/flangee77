#include "VertexBuffer.h"



namespace xl7::graphics::meshes {



    VertexBuffer::VertexBuffer(const CreateParams<Desc>& params)
        : MeshBuffer(Type::VertexBuffer, params, params.desc.stride)
        , _desc(params.desc)
    {
        assert(get_stride() > 0 && get_stride() >= _desc.vertex_layout.calculate_size());
    }



} // namespace xl7::graphics::meshes
