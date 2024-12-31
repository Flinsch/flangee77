#ifndef XL7_GRAPHICS_RENDERINGCONTEXT_H
#define XL7_GRAPHICS_RENDERINGCONTEXT_H

#include "./pipeline/Pipeline.h"

#include "./ClearFlags.h"
#include "./Color.h"



namespace xl7::graphics {



class RenderingDevice;



class RenderingContext
{

public:
    class Attorney
    {
        static void destroy(RenderingContext* rendering_context) { delete rendering_context; }
        friend class RenderingDevice;
    };



    RenderingContext() = delete;

    RenderingContext(const RenderingContext&) = delete;
    RenderingContext& operator=(const RenderingContext&) = delete;
    RenderingContext(RenderingContext&&) = delete;
    RenderingContext& operator=(RenderingContext&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * The rendering pipeline and its stages.
     */
    pipeline::Pipeline pipeline; // NOLINT(*-non-private-member-variables-in-classes)

    /**
     * Returns the owning rendering device.
     */
    RenderingDevice* get_rendering_device() const { return _rendering_device; }

    /**
     * Returns the owning rendering device implementation.
     */
    template <class TRenderingDeviceImpl>
    TRenderingDeviceImpl* get_rendering_device_impl() const
    {
        assert(static_cast<TRenderingDeviceImpl*>(_rendering_device) == dynamic_cast<TRenderingDeviceImpl*>(_rendering_device));
        return static_cast<TRenderingDeviceImpl*>(_rendering_device);
    }

    /**
     * Returns the 0-based index of the context (0: primary context).
     */
    unsigned get_index() const { return _index; }

    /**
     * Indicates whether this is the primary rendering context.
     */
    bool is_primary() const { return _index == 0; }

    /**
     * Returns the flag indicating whether the function `begin_scene` has been
     * called without a following call to `end_scene`.
     */
    bool is_scene_on() const { return _the_scene_is_on; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Performs a forced synchronization with the hardware state.
     * This function is called automatically after the rendering context has been
     * created and does not actually need to be used any further, perhaps after
     * resetting the rendering device, that's it.
     */
    bool synchronize_hardware_state();

    /**
     * Begins a scene.
     */
    bool begin_scene();

    /**
     * Ends a scene that was begun by calling `begin_scene`.
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



protected:
    struct ResolvedTargetStates
    {
        unsigned target_count;
        const surfaces::ColorRenderTarget* color_render_targets[pipeline::OutputMergerStage::MAX_RENDER_TARGETS];
        const surfaces::DepthStencilTarget* depth_stencil_target;
    };

    struct ResolvedTextureSamplerStates
    {
        unsigned texture_sampler_count;
        const textures::Texture* textures[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS];
        const states::SamplerState* sampler_states[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS];
    };

    struct ResolvedAbstractShaderStates
        : public ResolvedTextureSamplerStates
    {
        unsigned constant_buffer_count;
        const shaders::ConstantBuffer* constant_buffers[pipeline::AbstractShaderStage::MAX_CONSTANT_BUFFER_SLOTS];
        const shaders::ConstantBufferMapping* constant_buffer_mappings[pipeline::AbstractShaderStage::MAX_CONSTANT_BUFFER_SLOTS];
        const shaders::Shader* abstract_shader;
    };

    template <class TShader>
    struct ResolvedShaderStates
        : public ResolvedAbstractShaderStates
    {
        const TShader* shader;
    };

    struct ResolvedDrawStates
        : public ResolvedTargetStates
    {
        unsigned stream_count;
        const meshes::VertexBuffer* vertex_buffers[pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS];
        const meshes::IndexBuffer* index_buffer;
        meshes::Topology topology;

        ResolvedShaderStates<shaders::VertexShader> vs;
        ResolvedShaderStates<shaders::PixelShader> ps;

        Viewport viewport;

        const states::RasterizerState* rasterizer_state;
        const states::DepthStencilState* depth_stencil_state;
        const states::BlendState* blend_state;

        unsigned stencil_reference_value;
        Color blend_factor;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingContext(RenderingDevice* rendering_device, unsigned index);

    virtual ~RenderingContext() = default;



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Performs a forced synchronization with the hardware state.
     */
    virtual bool _synchronize_hardware_state_impl() = 0;

    /**
     * Begins a scene.
     */
    virtual bool _begin_scene_impl() = 0;

    /**
     * Ends a scene that was begun by calling `begin_scene`.
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

    /**
     * Gathers render targets (including resolving resource IDs into usable objects).
     */
    void _resolve_target_states(ResolvedTargetStates& resolved_target_states);

    /**
     *
     */
    template <class TShader>
    friend void _resolve_shader_states(RenderingDevice* _rendering_device, RenderingContext::ResolvedShaderStates<TShader>& resolved_shader_states, pipeline::AbstractShaderStage& pipeline_as);

    /**
     * Gathers drawing states (including resolving resource IDs into usable objects).
     */
    void _resolve_draw_states(ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced);

    /**
     * Validates the specified drawing states.
     */
    static bool _validate_resolved_draw_states(const ResolvedDrawStates& resolved_draw_states, bool indexed, bool instanced);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The owning rendering device.
     */
    RenderingDevice* const _rendering_device;

    /**
     * The 0-based index of the context (0: primary context).
     */
    const unsigned _index;

    /**
     * The flag indicating whether the function begin_scene has been called without
     * a following call to end_scene.
     */
    bool _the_scene_is_on;

}; // class RenderingContext



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_RENDERINGCONTEXT_H
