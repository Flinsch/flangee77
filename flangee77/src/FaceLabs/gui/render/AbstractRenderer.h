#ifndef FL7_GUI_RENDER_ABSTRACTRENDERER_H
#define FL7_GUI_RENDER_ABSTRACTRENDERER_H

#include "./BackgroundHelper.h"
#include "./TextHelper.h"

#include <FaceLabs/fonts/Font.h>
#include <FaceLabs/fonts/TextStyle.h>

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/textures/Texture2D.h>

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <memory>
#include <vector>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::render {



/**
 * Abstract drawing/rendering interface for a whole `Face` tree. A concrete
 * implementation (see `DefaultRenderer`) owns the tree traversal (order/strategy,
 * e.g., back-to-front for transparency, front-to-back with depth/stencil early-out
 * for opaque content, render-to-texture-per-face caching, ...) and the actual
 * drawing primitives (`_draw_rect_impl` etc.). How a *single* face is structured
 * (e.g., background before text, always back-to-front) is fixed and backend-
 * agnostic, though: see `_draw_face`, which delegates to per-aspect helpers (see
 * `BackgroundHelper`/`TextHelper`) that only ever talk to this class's own public
 * drawing methods, so that knowledge is written once here, not per backend.
 * `Face`/`Shell` themselves know nothing about drawing at all, they only expose
 * read-only structure (position, size, visibility, children, etc.) for a renderer
 * to look at.
 */
class AbstractRenderer
{

public:
    /**
     * A RAII helper for managing a frame in a scoped way.
     */
    class ScopedFrame
    {
    public:
        explicit ScopedFrame(AbstractRenderer* renderer)
            : _renderer(renderer)
        {
            if (_renderer)
                _renderer->begin_frame();
        }

        ScopedFrame(const ScopedFrame&) = delete;
        ScopedFrame& operator=(const ScopedFrame&) = delete;
        ScopedFrame(ScopedFrame&&) = delete;
        ScopedFrame& operator=(ScopedFrame&&) = delete;

        ~ScopedFrame()
        {
            if (_renderer)
                _renderer->end_frame();
        }

    private:
        AbstractRenderer* _renderer;
    };



    virtual ~AbstractRenderer() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Creates a new `ScopedFrame` object to manage a frame in a scoped way. The
     * returned `ScopedFrame` object will automatically call `end_frame` when it
     * goes out of scope.
     */
    ScopedFrame begin_scoped_frame() { return ScopedFrame(this); }

    /**
     * Begins a new frame. If you only call `draw_rect`, `draw_text`, etc.
     * occasionally, it is not necessary to call `begin_frame`/`end_frame` manually.
     * The renderer will manage this internally.
     *
     * For greater convenience, the RAII helper `ScopedFrame` should be used via
     * `begin_scoped_frame`.
     */
    void begin_frame();

    /**
     * Ends the current frame, submitting any queued/batched geometry.
     *
     * For greater convenience, the RAII helper `ScopedFrame` should be used`via
     * `begin_scoped_frame`, which ultimately calls `end_frame` automatically.
     */
    void end_frame();



    /**
     * Renders the given top-level faces (and, transitively, their descendants) as a
     * complete frame. This is the single entry point through which a shell gets
     * its faces drawn. Traversal order/strategy is entirely up to the concrete
     * renderer.
     */
    void render_faces(const std::vector<std::unique_ptr<Face>>& top_level_faces);



    /**
     * Draws a flat-colored, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see `push_clip_rect`).
     */
    void draw_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color);

    /**
     * Draws a simple-textured, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see `push_clip_rect`).
     * The sampled texture color is modulated by `tint`.
     */
    void draw_textured_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint);

    /**
     * Draws text at the specified position (screen pixel space, y-down). See
     * `fl7::fonts::render::AbstractRenderer::draw_text` for the exact
     * positioning/alignment contract.
     */
    void draw_text(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position);

    /**
     * Draws text laid out within the specified box (screen pixel space, y-down).
     * See `fl7::fonts::render::AbstractRenderer::draw_text_in_box` for the exact
     * positioning/alignment/wrapping contract.
     */
    void draw_text_in_box(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size);



    /**
     * Pushes a new clip rect, intersected with the current one (there is no clip
     * rect initially, i.e., unbounded). Drawing while this clip rect is active is
     * clipped against it.
     */
    void push_clip_rect(ml7::Vector2f clip_min, ml7::Vector2f clip_max);

    /**
     * Pops the most recently pushed clip rect, restoring the previous one.
     */
    void pop_clip_rect();



protected:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Draws the given face itself (not its children, if any, that's up to each
     * backend's own traversal in `_render_faces_impl`): its background (see
     * `BackgroundHelper`) if it's a `HasBackground`, its text (see `TextHelper`) if
     * it's a `HasText`, always in that (back-to-front) order. Backend-agnostic
     * (talks only to this class's own public drawing methods above), so shared by
     * every `AbstractRenderer` implementation. Call this from within your own
     * traversal wherever you decide to actually draw a given face.
     */
    void _draw_face(const Face& face, ml7::Vector2f absolute_position);



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    virtual void _begin_frame_impl() = 0;
    virtual void _end_frame_impl() = 0;

    virtual void _render_faces_impl(const std::vector<std::unique_ptr<Face>>& top_level_faces) = 0;

    virtual void _draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color) = 0;
    virtual void _draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint) = 0;
    virtual void _draw_text_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f position) = 0;
    virtual void _draw_text_in_box_impl(cl7::u8string_view text, fonts::Font* font, const fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size) = 0;

    virtual void _push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max) = 0;
    virtual void _pop_clip_rect_impl() = 0;



    // #############################################################################
    // Attributes
    // #############################################################################

    BackgroundHelper _background_helper;
    TextHelper _text_helper;

    /** The "flag"/counter specifying whether an active frame is open. */
    unsigned _frame_depth = 0;

}; // class AbstractRenderer



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_ABSTRACTRENDERER_H
