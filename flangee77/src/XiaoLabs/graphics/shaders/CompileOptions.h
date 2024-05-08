#pragma once
#ifndef XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
#define XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H

#include <CoreLabs/string.h>

#include <map>



namespace xl7 {
namespace graphics {
namespace shaders {



struct CompileOptions
{

    typedef std::map<cl7::astring, cl7::astring> MacroDefinitions;

    /** . */
    MacroDefinitions macro_definitions;

    /** The default entry point for vertex shaders. */
    cl7::astring default_vertex_entry_point = "mainVertex";
    /** The default entry point for pixel shaders. */
    cl7::astring default_pixel_entry_point = "mainPixel";

}; // struct CompileOptions



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
