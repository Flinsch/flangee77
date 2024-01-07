#include "VertexBuffer.h"

#include "./MeshManager.h"



namespace xl7 {
namespace graphics {
namespace meshes {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexBuffer::VertexBuffer(const CreateParams<Desc>& params)
        : Resource( params )
        , _desc( params.desc )
        , _stride( _desc.vertex_stride )
        , _size( _stride * _desc.vertex_count )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
