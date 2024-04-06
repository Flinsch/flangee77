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
        , pipeline()
        , _the_scene_is_on( false )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Performs a forced synchronization with the hardware state.
     * This function is called automatically after the rendering context has been
     * created and does not actually need to be used any further, perhaps after
     * resetting the rendering device, that's it.
     */
    bool RenderingContext::synchronize_hardware_state()
    {
        return _synchronize_hardware_state_impl();
    }

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
     * Clears the currently bound render target(s).
     */
    bool RenderingContext::clear(ClearFlags clear_flags, const Color& color, float depth, unsigned stencil)
    {
        ResolvedTargetStates resolved_target_states;
        _resolve_target_states( resolved_target_states );

        return _clear_impl( resolved_target_states, clear_flags, color, depth, stencil );
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContext::draw()
    {
        ResolvedDrawStates resolved_draw_states;
        _resolve_draw_states( resolved_draw_states, false, false );

        if ( !_validate_resolved_draw_states( resolved_draw_states, false, false ) )
            return false;

        assert( resolved_draw_states.vertex_buffers[0] );
        unsigned primitive_count = resolved_draw_states.vertex_buffers[0]->get_primitive_count();

        return _draw_impl( resolved_draw_states, primitive_count, 0 );
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContext::draw(meshes::Topology topology, unsigned primitive_count, unsigned start_vertex)
    {
        ResolvedDrawStates resolved_draw_states;
        _resolve_draw_states( resolved_draw_states, false, false );

        resolved_draw_states.topology = topology;

        if ( !_validate_resolved_draw_states( resolved_draw_states, false, false ) )
            return false;

        return _draw_impl( resolved_draw_states, primitive_count, start_vertex );
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContext::draw_indexed()
    {
        ResolvedDrawStates resolved_draw_states;
        _resolve_draw_states( resolved_draw_states, true, false );

        if ( !_validate_resolved_draw_states( resolved_draw_states, true, false ) )
            return false;

        assert( resolved_draw_states.index_buffer );
        unsigned primitive_count = resolved_draw_states.index_buffer->get_primitive_count();

        return _draw_indexed_impl( resolved_draw_states, primitive_count, 0, 0 );
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContext::draw_indexed(meshes::Topology topology, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        ResolvedDrawStates resolved_draw_states;
        _resolve_draw_states( resolved_draw_states, true, false );

        resolved_draw_states.topology = topology;

        if ( !_validate_resolved_draw_states( resolved_draw_states, true, false ) )
            return false;

        return _draw_indexed_impl( resolved_draw_states, primitive_count, start_index, base_vertex );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Gathers render targets (including resolving resource IDs into usable objects).
     */
    void RenderingContext::_resolve_target_states(ResolvedTargetStates& resolved_target_states)
    {
        resolved_target_states.target_count = 0;
        for ( unsigned target_index = 0; target_index < pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS; ++target_index )
        {
            auto* color_render_target = _rendering_device->get_surface_manager()->find_resource<surfaces::ColorRenderTarget>( pipeline.om.get_color_render_target_id( target_index ) );
            resolved_target_states.color_render_targets[ target_index ] = color_render_target;
            if ( color_render_target )
                resolved_target_states.target_count = target_index + 1;
        }

        resolved_target_states.depth_stencil_target = _rendering_device->get_surface_manager()->find_resource<surfaces::DepthStencilTarget>( pipeline.om.get_depth_stencil_target_id() );
    }

    /**
     * 
     */
    template <class TShader>
    static void _resolve_shader_states(RenderingContext::ResolvedShaderStates<TShader>& resolved_shader_states, RenderingDevice* const _rendering_device, pipeline::AbstractShaderStage& pipeline_as)
    {
        resolved_shader_states.shader = _rendering_device->get_shader_manager()->find_resource<TShader>( pipeline_as.get_shader_id() );

        resolved_shader_states.texture_sampler_count = 0;
        for ( unsigned slot_index = 0; slot_index < pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS; ++slot_index )
        {
            auto* texture = _rendering_device->get_texture_manager()->find_resource<textures::Texture>( pipeline_as.get_texture_id( slot_index ) );
            resolved_shader_states.textures[ slot_index ] = texture;
            if ( texture )
                resolved_shader_states.texture_sampler_count = slot_index + 1;

            auto* sampler_state = _rendering_device->get_state_manager()->find_resource<states::SamplerState>( pipeline_as.get_sampler_state_id( slot_index ) );
            if ( !sampler_state )
                sampler_state = _rendering_device->get_state_manager()->get_default_sampler_state();
            assert( sampler_state );
            resolved_shader_states.sampler_states[ slot_index ] = sampler_state;
        }
    }

    /**
     * Gathers drawing states (including resolving resource IDs into usable objects).
     */
    void RenderingContext::_resolve_draw_states(ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced)
    {
        _resolve_target_states( resolved_draw_states );

        meshes::Topology default_topology = meshes::Topology::Undefined;

        resolved_draw_states.stream_count = 0;
        for ( unsigned stream_index = 0; stream_index < pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS; ++stream_index )
        {
            auto* vertex_buffer = _rendering_device->get_mesh_manager()->find_resource<meshes::VertexBuffer>( pipeline.ia.get_vertex_buffer_id( stream_index ) );
            resolved_draw_states.vertex_buffers[ stream_index ] = vertex_buffer;
            if ( vertex_buffer )
                resolved_draw_states.stream_count = stream_index + 1;
            if ( vertex_buffer && stream_index == 0 )
                default_topology = vertex_buffer->get_desc().topology;
        }

        if ( indexed )
        {
            resolved_draw_states.index_buffer = _rendering_device->get_mesh_manager()->find_resource<meshes::IndexBuffer>( pipeline.ia.get_index_buffer_id() );
            resolved_draw_states.topology = pipeline.ia.get_topology( resolved_draw_states.index_buffer ? resolved_draw_states.index_buffer->get_desc().topology : default_topology );
        }
        else // => !indexed
        {
            resolved_draw_states.index_buffer = nullptr;
            resolved_draw_states.topology = pipeline.ia.get_topology( default_topology );
        } // indexed?

        _resolve_shader_states( resolved_draw_states.vs, _rendering_device, pipeline.vs );
        _resolve_shader_states( resolved_draw_states.ps, _rendering_device, pipeline.ps );

        resolved_draw_states.rasterizer_state = _rendering_device->get_state_manager()->find_resource<states::RasterizerState>( pipeline.rs.get_rasterizer_state_id() );
        resolved_draw_states.depth_stencil_state = _rendering_device->get_state_manager()->find_resource<states::DepthStencilState>( pipeline.om.get_depth_stencil_state_id() );
        resolved_draw_states.blend_state = _rendering_device->get_state_manager()->find_resource<states::BlendState>( pipeline.om.get_blend_state_id() );

        resolved_draw_states.stencil_reference_value = pipeline.om.get_stencil_reference_value();
        resolved_draw_states.blend_factor = pipeline.om.get_blend_factor();
    }

    /**
     * Validates the specified drawing states.
     */
    bool RenderingContext::_validate_resolved_draw_states(const ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced)
    {
        if ( instanced )
        {
            if ( resolved_draw_states.stream_count < 2 )
            {
                LOG_ERROR( TEXT("Too few vertex buffers have been set. Instanced rendering requires at least two vertex buffers.") );
                return false;
            }
        }
        else // => !instanced
        {
            if ( resolved_draw_states.stream_count < 1 )
            {
                LOG_ERROR( TEXT("No vertex buffer has been set.") );
                return false;
            }
        } // instanced?

        if ( indexed )
        {
            if ( !resolved_draw_states.index_buffer )
            {
                LOG_ERROR( TEXT("No index buffer has been set.") );
                return false;
            }
        }

        if ( resolved_draw_states.topology == meshes::Topology::Undefined )
        {
            LOG_ERROR( TEXT("No/unknown topology is provided.") );
            return false;
        }

        if ( !resolved_draw_states.vs.shader )
        {
            LOG_ERROR( TEXT("No vertex shader has been set.") );
            return false;
        }

        if ( !resolved_draw_states.ps.shader )
        {
            LOG_ERROR( TEXT("No pixel shader has been set.") );
            return false;
        }

        if ( resolved_draw_states.rasterizer_state && resolved_draw_states.rasterizer_state->get_desc().fill_mode == states::RasterizerState::FillMode::None )
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
