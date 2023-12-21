#include "StreamStates.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Properties
    // #############################################################################

    meshes::VertexBuffer* StreamStates::get_vertex_buffer(unsigned index) const
    {
        if ( index >= MAX_VERTEX_STREAMS )
            return nullptr;

        return _vertex_buffers.get_value( static_cast<size_t>( index ) );
    }

    void StreamStates::set_vertex_buffer(unsigned index, meshes::VertexBuffer* vertex_buffer)
    {
        if ( index >= MAX_VERTEX_STREAMS )
            return;

        _vertex_buffers.set_value( static_cast<size_t>( index ), vertex_buffer );
    }



} // namespace states
} // namespace graphics
} // namespace xl7
