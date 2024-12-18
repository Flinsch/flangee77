#pragma once
#ifndef XL7_GRAPHICS_VIEWPORT_H
#define XL7_GRAPHICS_VIEWPORT_H

#include <CoreLabs/root.h>



namespace xl7::graphics {



/** Defines the window dimensions of a render target onto which a 3D volume projects. */
struct Viewport
{

    /** The x pixel coordinate of the left hand side of the viewport. Default value: 0. */
    unsigned x      = 0;
    /** The y pixel coordinate of the top of the viewport. Default value: 0. */
    unsigned y      = 0;
    /** The width of the viewport, in pixels. Default value: width of the render target. */
    unsigned width  = 0;
    /** The height of the viewport, in pixels. Default value: height of the render target. */
    unsigned height = 0;
    /** The minimum depth of the viewport. Default value: 0. */
    float min_z     = 0.0f;
    /** The maximum depth of the viewport. Default value: 1. */
    float max_z     = 1.0f;

}; // struct Viewport



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_VIEWPORT_H
