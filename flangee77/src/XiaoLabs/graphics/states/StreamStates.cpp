#include "StreamStates.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Properties
    // #############################################################################

    resources::ResourceID StreamStates::get_vertex_buffer_id(unsigned index) const
    {
        if ( index >= MAX_VERTEX_STREAMS )
            return resources::ResourceID();

        return _vertex_buffer_ids.get_value( static_cast<size_t>( index ) );
    }

    void StreamStates::set_vertex_buffer_id(unsigned index, resources::ResourceID vertex_buffer_id)
    {
        if ( index >= MAX_VERTEX_STREAMS )
            return;

        _vertex_buffer_ids.set_value( static_cast<size_t>( index ), vertex_buffer_id );
    }



} // namespace states
} // namespace graphics
} // namespace xl7
