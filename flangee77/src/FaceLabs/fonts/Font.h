#ifndef FL7_FONTS_FONT_H
#define FL7_FONTS_FONT_H

#include "./Glyph.h"

#include <CoreLabs/strings/codec/codepoint.h>

#include <unordered_map>



namespace fl7::fonts {



/**
 * Represents a font holding all the glyphs and their outlines described as
 * quadratic Bézier curves.
 */
class Font
{

public:
    Font() = default;



    /**
     * Retrieves the glyph corresponding to the specified Unicode code point
     * (returns NULL if not found).
     */
    const Glyph* find_glyph(cl7::strings::codec::codepoint codepoint) const;



private:
    /** Maps Unicode code points to their respective glyphs. */
    std::unordered_map<cl7::strings::codec::codepoint::value_type, Glyph> _glyph_map;

}; // class Font



} // namespace fl7::fonts

#endif // FL7_FONTS_FONT_H
