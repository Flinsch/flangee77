#include "IndexBuffer.h"

#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



    IndexBuffer::IndexBuffer(const CreateContext& ctx, const IndexBufferDesc& desc)
        : ResourceBase(Type::IndexBuffer, ctx, {
            .usage = desc.usage,
            .topology = desc.topology,
            .count = desc.count,
        }, MeshUtil::get_index_stride(desc.index_type))
        , _desc(desc)
    {
        assert(static_cast<const void*>(&MeshBuffer::get_desc()) != static_cast<const void*>(&get_desc()));
        assert(::memcmp(&MeshBuffer::get_desc(), &get_desc(), sizeof(MeshBufferDesc)) == 0);

        assert(get_stride() == 2 || get_stride() == 4);
    }



} // namespace xl7::graphics::meshes
