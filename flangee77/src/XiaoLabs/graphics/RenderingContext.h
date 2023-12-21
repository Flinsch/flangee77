#pragma once
#ifndef XL7_GRAPHICS_RENDERINGCONTEXT_H
#define XL7_GRAPHICS_RENDERINGCONTEXT_H

#include "./states/StreamStates.h"
#include "./states/RenderStates.h"



namespace xl7 {
namespace graphics {



class RenderingContext
{

public:
    class Attorney
    {
        static void destroy(RenderingContext* rendering_context) { delete rendering_context; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingContext(unsigned index);

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
     * The 0-based index of the context (0: primary context).
     */
    const unsigned _index;

public:
    /**
     * The stream states for the pipeline stage of the input assembler.
     */
    states::StreamStates stream_states;

    /**
     * The render states for the pipeline stage of the rasterizer.
     */
    states::RenderStates render_states;

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

}; // class RenderingContext



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_RENDERINGCONTEXT_H
