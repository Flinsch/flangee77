#include "IndexBuffer.h"

#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



    IndexBuffer::IndexBuffer(const CreateParams<Desc>& params)
        : MeshBufferBase(Type::IndexBuffer, params, MeshUtil::get_index_stride(params.desc.index_type))
        , _desc(params.desc)
    {
        assert(get_stride() == 2 || get_stride() == 4);
    }



} // namespace xl7::graphics::meshes
