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
    VertexBuffer::VertexBuffer(MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : Resource( manager, identifier )
        , desc( desc )
        , size( desc.vertex_stride * desc.vertex_count )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
