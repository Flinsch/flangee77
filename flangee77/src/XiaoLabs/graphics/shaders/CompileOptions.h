#pragma once
#ifndef XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
#define XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H

#include <CoreLabs/string.h>

#include <map>



namespace xl7::graphics::shaders {



struct CompileOptions
{

    using MacroDefinitions = std::map<cl7::u8string, cl7::u8string>;

    /** The macro definitions for (re)compiling shaders. */
    MacroDefinitions macro_definitions;

    /** The default entry point for (re)compiling vertex shaders. */
    cl7::u8string default_vertex_entry_point = u8"mainVertex";
    /** The default entry point for (re)compiling pixel shaders. */
    cl7::u8string default_pixel_entry_point = u8"mainPixel";

}; // struct CompileOptions



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_COMPILEOPTIONS_H
