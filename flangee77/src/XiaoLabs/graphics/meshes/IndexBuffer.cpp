#include "IndexBuffer.h"

#include "./MeshManager.h"
#include "./MeshUtil.h"



namespace xl7 {
namespace graphics {
namespace meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    IndexBuffer::IndexBuffer(const CreateParams<Desc>& params)
        : Resource( params )
        , _desc( params.desc )
        , _stride( MeshUtil::calculate_index_stride( _desc.index_type ) )
        , _size( _stride * _desc.index_count )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
