#ifndef FL7_FONTS_ICONRUN_H
#define FL7_FONTS_ICONRUN_H

#include "./Icon.h"



namespace fl7::fonts {



/**
 * Places an Icon at a single code point position (slot) within a single draw call.
 * The code point actually present at that position in the text is skipped/ignored:
 * the IconRun entirely overrides both its advance width (layout) and its visuals.
 *
 * `icon_runs` passed to a single draw call must be sorted by `codepoint_index`
 * (same convention as `StyleRun`'s `codepoint_begin`).
 */
struct IconRun
{
    /** The code point index (slot) this icon occupies. */
    size_t codepoint_index = 0;

    /** The icon to draw. Not owned: must outlive the draw call. */
    const Icon* icon = nullptr;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_ICONRUN_H
