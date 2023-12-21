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
    IndexBuffer::IndexBuffer(MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : Resource( manager, identifier )
        , desc( desc )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
