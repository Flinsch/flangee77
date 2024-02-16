#include "VertexBufferBinding.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace meshes {



    VertexBufferBinding::VertexBufferBinding()
        : stream_count( 0 )
    {
    }

    bool VertexBufferBinding::operator == (const VertexBufferBinding& rhs) const
    {
        if ( stream_count != rhs.stream_count )
            return false;

        for ( unsigned i = 0; i < stream_count; ++i )
            if ( vertex_buffer_ids[ i ] != rhs.vertex_buffer_ids[ i ] )
                return false;

        return true;
    }

    size_t VertexBufferBinding::hash() const
    {
        size_t hash = static_cast<size_t>( stream_count );

        for ( unsigned i = 0; i < stream_count; ++i )
            hash ^= static_cast<size_t>( vertex_buffer_ids[ i ].value() ) << (i + 1);

        return hash;
    }



} // namespace meshes
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7
