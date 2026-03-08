#include "IndexBuffer.h"

#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



    IndexBuffer::IndexBuffer(const CreateContext& ctx, const IndexBufferDesc& desc)
        : ResourceBase(Type::IndexBuffer, ctx, desc, MeshUtil::get_index_stride(desc.index_type))
        , _desc(desc)
    {
        assert(get_stride() == 2 || get_stride() == 4);
    }



} // namespace xl7::graphics::meshes
