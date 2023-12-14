#include "IndexBuffer.h"

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
    IndexBuffer::IndexBuffer(MeshManager* manager)
        : Resource( manager )
    {
    }

    /**
     * Explicit constructor.
     */
    IndexBuffer::IndexBuffer(MeshManager* manager, const cl7::string& identifier)
        : Resource( manager, identifier )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
