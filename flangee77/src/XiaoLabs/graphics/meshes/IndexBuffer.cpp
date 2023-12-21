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
    IndexBuffer::IndexBuffer(MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : Resource( manager, identifier )
        , desc( desc )
        , stride( MeshUtil::calculate_index_stride( desc.index_type ) )
        , size( stride * desc.index_count )
    {
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
