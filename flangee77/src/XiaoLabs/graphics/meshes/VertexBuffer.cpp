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
    VertexBuffer::VertexBuffer(MeshManager* manager)
        : Resource( manager )
    {
    }

    /**
     * Explicit constructor.
     */
    VertexBuffer::VertexBuffer(MeshManager* manager, const cl7::string& identifier)
        : Resource( manager, identifier )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
