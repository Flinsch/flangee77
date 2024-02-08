#include "RenderingContext.h"

#include "./GraphicsSystem.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/memory.h>



namespace xl7 {
namespace graphics {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContext::RenderingContext(RenderingDevice* rendering_device, unsigned index)
        : _rendering_device( rendering_device )
        , _index( index )
        , stream_states()
        , vertex_shader_states()
        , pixel_shader_states()
        , render_states()
        , _the_scene_is_on( false )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContext::begin_scene()
    {
        if ( _the_scene_is_on )
        {
            if ( _index == 0 )
                LOG_WARNING( TEXT("The scene of the primary rendering context is already on.") );
            else
                LOG_WARNING( TEXT("The scene of the rendering context with 0-based index ") + cl7::to_string(_index) + TEXT(" is already on.") );
            return false;
        }

        if ( !_begin_scene_impl() )
            return false;

        _the_scene_is_on = true;
        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContext::end_scene()
    {
        if ( !_the_scene_is_on )
        {
            if ( _index == 0 )
                LOG_WARNING( TEXT("The scene of the primary rendering context is not on.") );
            else
                LOG_WARNING( TEXT("The scene of the rendering context with 0-based index ") + cl7::to_string(_index) + TEXT(" is not on.") );
            return false;
        }

        if ( !_end_scene_impl() )
            return false;

        _the_scene_is_on = false;
        return true;
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContext::draw()
    {
        DrawStates draw_states;
        _gather_draw_states( draw_states, false, false );

        if ( !_validate_draw_states( draw_states, false, false ) )
            return false;

        assert( draw_states.vertex_buffers[0] );
        unsigned primitive_count = draw_states.vertex_buffers[0]->get_primitive_count();

        return _draw_impl( draw_states, primitive_count, 0 );
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContext::draw(meshes::Topology topology, unsigned primitive_count, unsigned start_vertex)
    {
        DrawStates draw_states;
        _gather_draw_states( draw_states, false, false );

        draw_states.topology = topology;

        if ( !_validate_draw_states( draw_states, false, false ) )
            return false;

        return _draw_impl( draw_states, primitive_count, start_vertex );
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContext::draw_indexed()
    {
        DrawStates draw_states;
        _gather_draw_states( draw_states, true, false );

        if ( !_validate_draw_states( draw_states, true, false ) )
            return false;

        assert( draw_states.index_buffer );
        unsigned primitive_count = draw_states.index_buffer->get_primitive_count();

        return _draw_indexed_impl( draw_states, primitive_count, 0, 0 );
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContext::draw_indexed(meshes::Topology topology, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        DrawStates draw_states;
        _gather_draw_states( draw_states, true, false );

        draw_states.topology = topology;

        if ( !_validate_draw_states( draw_states, true, false ) )
            return false;

        return _draw_indexed_impl( draw_states, primitive_count, start_index, base_vertex );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Gathers drawing states (including resolving resource IDs into usable objects).
     */
    void RenderingContext::_gather_draw_states(DrawStates& draw_states, bool indexed, bool instanced)
    {
        draw_states.stream_count = 0;
        for ( unsigned stream_index = 0; stream_index < states::StreamStates::MAX_VERTEX_STREAMS; ++stream_index )
        {
            auto* vertex_buffer = _rendering_device->get_mesh_manager()->find_resource<meshes::VertexBuffer>( stream_states.get_vertex_buffer_id( stream_index ) );
            if ( !vertex_buffer )
                break;
            draw_states.vertex_buffers[ stream_index ] = vertex_buffer;
            ++draw_states.stream_count;
        }

        if ( indexed )
        {
            draw_states.index_buffer = _rendering_device->get_mesh_manager()->find_resource<meshes::IndexBuffer>( stream_states.get_index_buffer_id() );
            draw_states.topology = stream_states.get_topology( draw_states.index_buffer->get_desc().topology );
        }
        else // => !indexed
        {
            draw_states.index_buffer = nullptr;
            draw_states.topology = stream_states.get_topology( draw_states.vertex_buffers[0]->get_desc().topology );
        } // indexed?

        draw_states.vertex_shader = _rendering_device->get_shader_manager()->find_resource<shaders::VertexShader>( vertex_shader_states.get_shader_id() );
        draw_states.pixel_shader = _rendering_device->get_shader_manager()->find_resource<shaders::PixelShader>( pixel_shader_states.get_shader_id() );
    }

    /**
     * Validates the specified drawing states.
     */
    bool RenderingContext::_validate_draw_states(const DrawStates& draw_states, bool indexed, bool instanced)
    {
        if ( instanced )
        {
            if ( draw_states.stream_count < 2 )
            {
                LOG_ERROR( TEXT("Too few vertex buffers have been set. Instanced rendering requires at least two vertex buffers.") );
                return false;
            }
        }
        else // => !instanced
        {
            if ( draw_states.stream_count < 1 )
            {
                LOG_ERROR( TEXT("No vertex buffer has been set.") );
                return false;
            }
        } // instanced?

        if ( indexed )
        {
            if ( !draw_states.index_buffer )
            {
                LOG_ERROR( TEXT("No index buffer has been set.") );
                return false;
            }
        }

        if ( draw_states.topology == meshes::Topology::Unknown )
        {
            LOG_ERROR( TEXT("No/unknown topology is provided.") );
            return false;
        }

        if ( !draw_states.vertex_shader )
        {
            LOG_ERROR( TEXT("No vertex shader has been set.") );
            return false;
        }

        if ( !draw_states.pixel_shader )
        {
            LOG_ERROR( TEXT("No pixel shader has been set.") );
            return false;
        }

        if ( render_states.get_fill_mode() == states::RenderStates::FillMode::None )
        {
            // Just return false without issuing an error message.
            // Because it's not an error in the actual sense;
            // it's just that nothing should be drawn.
            return false;
        }

        return true;
    }



} // namespace graphics
} // namespace xl7
