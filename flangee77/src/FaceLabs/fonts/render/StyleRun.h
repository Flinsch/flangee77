#ifndef FL7_FONTS_RENDER_STYLERUN_H
#define FL7_FONTS_RENDER_STYLERUN_H

#include "../GlyphStyle.h"



namespace fl7::fonts::render {



/**
 * Describes a style override for a sub-range of code points within a single draw
 * call, on top of the base TextStyle's (inherited) GlyphStyle.
 *
 * `codepoint_begin`/`codepoint_end` use the same indexing convention as TextLine:
 * they index into the caller's original code point sequence. Runs passed to a
 * single draw call must be sorted by `codepoint_begin` and must not overlap.
 */
struct StyleRun
{
    /** The index of the first code point this run applies to (inclusive). */
    size_t codepoint_begin = 0;
    /** The index of the first code point after this run (exclusive). */
    size_t codepoint_end = 0;

    /** The per-glyph style to use for this run, overriding the base TextStyle's. */
    GlyphStyle style;
};



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_STYLERUN_H
