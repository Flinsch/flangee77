#pragma once
#ifndef XL7_GRAPHICS_RENDERINGCONTEXT_H
#define XL7_GRAPHICS_RENDERINGCONTEXT_H

#include "./states/StreamStates.h"
#include "./states/ShaderStates.h"
#include "./states/RenderStates.h"
#include "./states/TargetStates.h"

#include "./ClearFlags.h"
#include "./Color.h"



namespace xl7 {
namespace graphics {



class RenderingDevice;



class RenderingContext
{

public:
    class Attorney
    {
        static void destroy(RenderingContext* rendering_context) { delete rendering_context; }
        friend class RenderingDevice;
    };

protected:
    struct ResolvedTargetStates
    {
        unsigned target_count;
        const surfaces::ColorRenderTarget* color_render_targets[ states::TargetStates::MAX_RENDER_TARGETS ];
        const surfaces::DepthStencilTarget* depth_stencil_target;
    };

    struct ResolvedDrawStates
        : public ResolvedTargetStates
    {
        unsigned stream_count;
        const meshes::VertexBuffer* vertex_buffers[ states::StreamStates::MAX_VERTEX_STREAMS ];
        const meshes::IndexBuffer* index_buffer;
        meshes::Topology topology;

        const shaders::VertexShader* vertex_shader;
        const shaders::PixelShader* pixel_shader;

        const states::ShaderStates* render_states;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingContext(RenderingDevice* rendering_device, unsigned index);

    /**
     * Destructor.
     */
    virtual ~RenderingContext() = default;

private:
    /** Default constructor. */
    RenderingContext();
    /** Copy constructor. */
    RenderingContext(const RenderingContext&) = delete;
    /** Copy assignment operator. */
    RenderingContext& operator = (const RenderingContext&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The owning rendering device.
     */
    RenderingDevice* const _rendering_device;

    /**
     * The 0-based index of the context (0: primary context).
     */
    const unsigned _index;

public:
    /**
     * The stream states for the pipeline stage of the input assembler.
     */
    states::StreamStates stream_states;

    /**
     * The shader states for the pipeline stage of the vertex shader.
     */
    states::ShaderStates vertex_shader_states;

    /**
     * The shader states for the pipeline stage of the pixel shader.
     */
    states::ShaderStates pixel_shader_states;

    /**
     * The render states for the pipeline stage of the rasterizer.
     */
    states::RenderStates render_states;

    /**
     * The target states for the pipeline stage of the output merger.
     */
    states::TargetStates target_states;

private:
    /**
     * The flag indicating whether the function begin_scene has been called without
     * a following call to end_scene.
     */
    bool _the_scene_is_on;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the owning rendering device.
     */
    RenderingDevice* get_rendering_device() const { return _rendering_device; }

    /**
     * Returns the 0-based index of the context (0: primary context).
     */
    unsigned get_index() const { return _index; }

    /**
     * Indicates whether this is the primary rendering context.
     */
    bool is_primary() const { return _index == 0; }

public:
    /**
     * Returns the flag indicating whether the function begin_scene has been called
     * without a following call to end_scene.
     */
    bool is_scene_on() const { return _the_scene_is_on; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Begins a scene.
     */
    bool begin_scene();

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool end_scene();

    /**
     * Clears the currently bound render target(s).
     */
    bool clear(ClearFlags clear_flags, const Color& color, float depth, unsigned stencil);

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool draw();

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool draw(meshes::Topology topology, unsigned primitive_count, unsigned start_vertex = 0);

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool draw_indexed();

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool draw_indexed(meshes::Topology topology, unsigned primitive_count, unsigned start_index = 0, signed base_vertex = 0);

    /**
     * Draws non-indexed, instanced primitives.
     */
    //bool draw_instanced();

    /**
     * Draws indexed, instanced primitives.
     */
    //bool draw_indexed_instanced();



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Begins a scene.
     */
    virtual bool _begin_scene_impl() = 0;

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    virtual bool _end_scene_impl() = 0;

    /**
     * Clears the currently bound render target(s).
     */
    virtual bool _clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil) = 0;

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    virtual bool _draw_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_vertex) = 0;

    /**
     * Draws indexed, non-instanced primitives.
     */
    virtual bool _draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex) = 0;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Gathers render targets (including resolving resource IDs into usable objects).
     */
    void _resolve_target_states(ResolvedTargetStates& resolved_target_states);

    /**
    * Gathers drawing states (including resolving resource IDs into usable objects).
    */
    void _resolve_draw_states(ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced);

    /**
     * Validates the specified drawing states.
     */
    bool _validate_resolved_draw_states(const ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced);

}; // class RenderingContext



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGCONTEXT_H
