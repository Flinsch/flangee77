#include "Font.h"



namespace fl7::fonts {



    /**
     * Retrieves the glyph corresponding to the specified Unicode code point
     * (returns NULL if not found).
     */
    const Glyph* Font::find_glyph(cl7::strings::codepoint codepoint) const
    {
        auto it = _glyph_map.find(codepoint.value);
        if (it == _glyph_map.end())
            return nullptr;
        return &it->second;
    }



} // namespace fl7::fonts
