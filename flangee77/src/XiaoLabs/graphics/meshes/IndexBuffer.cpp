#include "IndexBuffer.h"

#include "./MeshUtil.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    /**
     * Explicit constructor.
     */
    IndexBuffer::IndexBuffer(const CreateParams<Desc>& params)
        : MeshBuffer(Type::IndexBuffer, params, MeshUtil::calculate_index_stride(params.desc.index_type))
        , _desc(params.desc)
    {
        assert(get_stride() == 2 || get_stride() == 4);
    }



} // namespace xl7::graphics::meshes
