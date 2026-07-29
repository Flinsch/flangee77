#ifndef XL7_GRAPHICS_QUADRENDERER_H
#define XL7_GRAPHICS_QUADRENDERER_H

#include "./Color.h"

#include "./meshes/VertexLayout.h"
#include "./meshes/VertexBuffer.h"
#include "./shaders/VertexShader.h"
#include "./shaders/PixelShader.h"
#include "./shaders/ConstantBuffer.h"
#include "./states/SamplerState.h"
#include "./states/BlendState.h"
#include "./states/DepthStencilState.h"
#include "./textures/Texture2D.h"

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <vector>



namespace xl7::graphics {



/**
 * Generic batched 2D quad renderer: flat-colored and simple-textured axis-aligned
 * rects, with clip-rect support.
 */
class QuadRenderer
{

public:
    /**
     * A RAII helper for managing quad rendering batches in a scoped and nestable
     * way.
     */
    class ScopedBatch
    {
    public:
        explicit ScopedBatch(QuadRenderer* renderer)
            : _renderer(renderer)
        {
            if (_renderer)
                _renderer->begin();
        }

        ScopedBatch(const ScopedBatch&) = delete;
        ScopedBatch& operator=(const ScopedBatch&) = delete;
        ScopedBatch(ScopedBatch&&) = delete;
        ScopedBatch& operator=(ScopedBatch&&) = delete;

        ~ScopedBatch()
        {
            if (_renderer)
                _renderer->end();
        }

        void draw_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, const Color& color)
        {
            if (_renderer)
                _renderer->draw_rect(position_min, position_max, color);
        }

        void draw_textured_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, textures::Texture2D::Id texture_id, ml7::Vector2f uv_min = {}, ml7::Vector2f uv_max = ml7::Vector2f{1.0f, 1.0f}, const Color& tint = Color::WHITE)
        {
            if (_renderer)
                _renderer->draw_textured_rect(position_min, position_max, texture_id, uv_min, uv_max, tint);
        }

        /**
         * Forces the submission of all currently batched geometry. This can be
         * called manually if external render state changes, or when mixing with
         * other rendering systems that may modify render state.
         */
        void flush()
        {
            if (_renderer)
                _renderer->flush();
        }

        /**
         * Submits any queued geometry and finalizes the current batch.
         * This implicitly calls flush.
         */
        void end()
        {
            if (_renderer)
                _renderer->end();
            _renderer = nullptr;
        }

    private:
        QuadRenderer* _renderer;
    };

    /**
     * A RAII helper for pushing/popping a clip rect in a scoped way.
     */
    class ScopedClipRect
    {
    public:
        ScopedClipRect(QuadRenderer* renderer, ml7::Vector2f clip_min, ml7::Vector2f clip_max)
            : _renderer(renderer)
        {
            if (_renderer)
                _renderer->push_clip_rect(clip_min, clip_max);
        }

        ScopedClipRect(const ScopedClipRect&) = delete;
        ScopedClipRect& operator=(const ScopedClipRect&) = delete;
        ScopedClipRect(ScopedClipRect&&) = delete;
        ScopedClipRect& operator=(ScopedClipRect&&) = delete;

        ~ScopedClipRect()
        {
            if (_renderer)
                _renderer->pop_clip_rect();
        }

    private:
        QuadRenderer* _renderer;
    };



    QuadRenderer();
    ~QuadRenderer();

    QuadRenderer(const QuadRenderer&) = delete;
    QuadRenderer& operator=(const QuadRenderer&) = delete;
    QuadRenderer(QuadRenderer&&) = delete;
    QuadRenderer& operator=(QuadRenderer&&) = delete;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Creates a new ScopedBatch object to manage a quad rendering batch. The
     * returned ScopedBatch object will automatically call end() when it goes out of
     * scope, unless end() is called explicitly beforehand.
     */
    ScopedBatch begin_batch() { return ScopedBatch(this); }

    /**
     * Begins a new quad rendering batch. If you only call draw_rect()/
     * draw_textured_rect() occasionally, it is not necessary to call begin()/
     * end() manually: the renderer will manage batching internally.
     *
     * For greater convenience, the RAII helper ScopedBatch should be used via
     * begin_batch().
     */
    void begin();

    /**
     * Submits any queued geometry and finalizes the current batch. This implicitly
     * calls flush().
     *
     * For greater convenience, the RAII helper ScopedBatch should be used via
     * begin_batch(), which ultimately calls end() automatically.
     */
    void end();

    /**
     * Forces the submission of all currently batched geometry. This can be called
     * manually if external render state changes, or when mixing with other
     * rendering systems that may modify render state.
     */
    void flush();



    /**
     * Draws a flat-colored, axis-aligned rect (screen pixel space, y-down), clipped
     * against the current clip rect (see push_clip_rect()).
     *
     * If no active batch is open, begin()/end() are called automatically.
     */
    void draw_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, const Color& color);

    /**
     * Draws a simple-textured, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see push_clip_rect()). The sampled
     * texture color is modulated by `tint`.
     *
     * If no active batch is open, begin()/end() are called automatically.
     */
    void draw_textured_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, textures::Texture2D::Id texture_id, ml7::Vector2f uv_min = {}, ml7::Vector2f uv_max = ml7::Vector2f{1.0f, 1.0f}, const Color& tint = Color::WHITE);



    /**
     * Pushes a new clip rect, intersected with the current one (there is no clip
     * rect initially, i.e., unbounded). Quads drawn while this clip rect is active
     * are clipped against it (see ClippedQuad::clip()): a quad clipped away
     * entirely is silently skipped.
     */
    void push_clip_rect(ml7::Vector2f clip_min, ml7::Vector2f clip_max);

    /**
     * Pops the most recently pushed clip rect, restoring the previous one (or the
     * unbounded default, if the stack is empty afterward).
     */
    void pop_clip_rect();

    /**
     * Creates a new ScopedClipRect object to manage a push_clip_rect()/
     * pop_clip_rect() pair in a scoped way.
     */
    ScopedClipRect push_scoped_clip_rect(ml7::Vector2f clip_min, ml7::Vector2f clip_max) { return ScopedClipRect(this, clip_min, clip_max); }



private:

    // #############################################################################
    // Types
    // #############################################################################

    struct Vertex
    {
        ml7::Vector2f position;
        ml7::Vector2f texcoord;
        Color color;
    };

    enum struct BatchKind
    {
        Solid,
        Textured,
    };

    struct DrawBatch
    {
        BatchKind kind;
        /** Only meaningful when kind == Textured. */
        textures::Texture2D::Id texture_id;
        unsigned first_vertex;
        unsigned vertex_count;
    };

    struct ClipRect
    {
        ml7::Vector2f min;
        ml7::Vector2f max;
    };



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Clips the specified quad against the current clip rect (if any) and, if
     * anything remains visible, appends it to the current batch.
     */
    void _emit_quad(ml7::Vector2f position_min, ml7::Vector2f position_max, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const Color& color, BatchKind kind, textures::Texture2D::Id texture_id);

    /**
     * Lazily creates the GPU resources (shaders, constant buffer, states) this
     * renderer needs, the first time they are needed.
     */
    void _ensure_gpu_resources();

    /**
     * Releases all GPU resources held by this renderer.
     */
    void _release_gpu_resources();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The "flag"/counter specifying whether an active batch is open. */
    unsigned _batch_depth = 0;

    std::vector<Vertex> _vertices;
    std::vector<DrawBatch> _batches;

    /** The active clip-rect stack; empty means unbounded (no clipping). */
    std::vector<ClipRect> _clip_rect_stack;

    /** Prefixed onto every GPU resource identifier this instance creates, so multiple QuadRenderer instances don't collide (see AbstractTextureAtlasBasedRenderer's identical pattern). */
    cl7::u8string _resource_prefix;

    meshes::VertexLayout _vertex_layout;

    meshes::VertexBuffer::Id _vertex_buffer_id = {};
    unsigned _vertex_buffer_capacity = 0;

    shaders::VertexShader::Id _vertex_shader_id = {};
    shaders::PixelShader::Id _solid_pixel_shader_id = {};
    shaders::PixelShader::Id _textured_pixel_shader_id = {};
    shaders::ConstantBuffer::Id _constant_buffer_id = {};

    states::SamplerState::Id _sampler_state_id = {};
    states::BlendState::Id _blend_state_id = {};
    states::DepthStencilState::Id _depth_stencil_state_id = {};

}; // class QuadRenderer



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_QUADRENDERER_H
