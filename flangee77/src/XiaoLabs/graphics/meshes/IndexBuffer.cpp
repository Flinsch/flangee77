#include "IndexBuffer.h"

#include "./MeshUtil.h"

#include <CoreLabs/logging.h>



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
        : MeshBuffer( params, MeshUtil::calculate_index_stride( _desc.index_type ) )
        , _desc( params.desc )
    {
        assert( _stride == 2 || _stride == 4 );
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
