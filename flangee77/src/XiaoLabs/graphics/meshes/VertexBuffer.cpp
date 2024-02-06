#include "VertexBuffer.h"

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
    VertexBuffer::VertexBuffer(const CreateParams<Desc>& params)
        : MeshBuffer( params, params.desc.stride )
        , _desc( params.desc )
    {
        assert( _stride > 0 && _stride >= _desc.vertex_layout.calculate_size() );
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
