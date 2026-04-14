#include "MeshBuffer.h"

#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



    MeshBuffer::MeshBuffer(const CreateContext& ctx, Type type, const MeshBufferDesc& desc)
        : ResourceWithData(ctx, static_cast<size_t>(desc.element_count * desc.element_stride))
        , _type(type)
        , _desc(desc)
        , _primitive_count(MeshUtil::calculate_primitive_count(desc.topology, desc.element_count))
    {
        assert(get_data_size() > 0);
    }



} // namespace xl7::graphics::meshes
