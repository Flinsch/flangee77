#ifndef XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
#define XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H

#include <CoreLabs/string.h>

#include <map>



namespace xl7::graphics::shaders {



struct CompileOptions
{

    using MacroDefinitions = std::map<cl7::astring, cl7::astring>;

    /** The macro definitions for (re)compiling shaders. */
    MacroDefinitions macro_definitions;

    /** The default entry point for (re)compiling vertex shaders. */
    cl7::astring default_vertex_entry_point = "mainVertex";
    /** The default entry point for (re)compiling pixel shaders. */
    cl7::astring default_pixel_entry_point = "mainPixel";

}; // struct CompileOptions



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
