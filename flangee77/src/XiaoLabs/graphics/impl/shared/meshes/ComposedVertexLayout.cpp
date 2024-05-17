#include "ComposedVertexLayout.h"

#include "../../../GraphicsSystem.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace meshes {



    ComposedVertexLayout::ComposedVertexLayout()
        : stream_count( 0 )
    {
    }

    ComposedVertexLayout::ComposedVertexLayout(const VertexBufferBinding& vertex_buffer_binding)
        : ComposedVertexLayout()
    {
        auto* mesh_manager = xl7::graphics::GraphicsSystem::instance().get_rendering_device()->get_mesh_manager();

        for ( unsigned stream_index = 0; stream_index < pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS; ++stream_index )
        {
            xl7::graphics::meshes::VertexBuffer* vertex_buffer;
            if ( vertex_buffer_binding.vertex_buffer_ids[ stream_index ].is_valid() )
                vertex_buffer = mesh_manager->find_resource<xl7::graphics::meshes::VertexBuffer>( vertex_buffer_binding.vertex_buffer_ids[ stream_index ] );
            else
                vertex_buffer = nullptr;

            if ( vertex_buffer ) {
                stream_count = stream_index + 1;
                instance_data_step_rates[ stream_index ] = vertex_buffer->get_desc().instance_data_step_rate;
                vertex_layouts[ stream_index ] = vertex_buffer->get_desc().vertex_layout;
            } else {
                instance_data_step_rates[ stream_index ] = 0;
                vertex_layouts[ stream_index ] = {};
            }
        } // for each vertex stream
    }

    bool ComposedVertexLayout::operator == (const ComposedVertexLayout& rhs) const
    {
        if ( stream_count != rhs.stream_count )
            return false;

        for ( unsigned i = 0; i < stream_count; ++i )
        {
            if ( instance_data_step_rates[ i ] != rhs.instance_data_step_rates[ i ] )
                return false;

            const auto& elements = vertex_layouts[ i ].elements;
            const auto& rhs_elements = rhs.vertex_layouts[ i ].elements;

            if ( elements.size() != rhs_elements.size() )
                return false;

            for ( size_t j = 0; j < elements.size(); ++j )
            {
                const auto& element = elements[ j ];
                const auto& rhs_element = rhs_elements[ j ];

                if ( element.semantic != rhs_element.semantic )
                    return false;
                if ( element.semantic_index != rhs_element.semantic_index )
                    return false;
                if ( element.data_type != rhs_element.data_type )
                    return false;
            } // for each vertex element
        } // for each vertex layout

        return true;
    }

    size_t ComposedVertexLayout::hash() const
    {
        size_t hash = static_cast<size_t>( stream_count );

        for ( unsigned i = 0; i < stream_count; ++i )
        {
            const auto& elements = vertex_layouts[ i ].elements;

            size_t h = elements.size() ^ ((instance_data_step_rates[ i ] * 100) % 79);

            for ( size_t j = 0; j < elements.size(); ++j )
            {
                const auto& element = elements[ j ];

                size_t h_ = 0;
                
                h_ ^= static_cast<size_t>( element.semantic );
                h_ ^= static_cast<size_t>( element.semantic_index ) << 1;
                h_ ^= static_cast<size_t>( element.data_type ) << 2;

                h ^= h_ << (j + 1);
            } // for each vertex element

            hash ^= h << (i + 1);
        } // for each vertex layout

        return hash;
    }



} // namespace meshes
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7
