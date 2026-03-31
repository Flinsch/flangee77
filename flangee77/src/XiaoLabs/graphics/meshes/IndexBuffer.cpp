#include "IndexBuffer.h"

#include "./MeshUtil.h"



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
    }



} // namespace xl7::graphics::meshes
