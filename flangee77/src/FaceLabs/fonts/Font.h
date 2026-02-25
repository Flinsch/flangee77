#ifndef FL7_FONTS_FONT_H
#define FL7_FONTS_FONT_H

#include "./FontLoader.h"
#include "./FontMetrics.h"
#include "./Glyph.h"

#include <CoreLabs/text/codec/codepoint.h>

#include <array>
#include <unordered_map>



namespace fl7::fonts {



/**
 * Represents a font holding all the glyphs and their outlines described as
 * quadratic Bézier curves.
 */
class Font
{

public:
    /**
     * Provides access to retrieve metrics and glyphs of the font.
     */
    class Access
    {
    public:
        Access(const Access&) = delete;
        Access& operator=(const Access&) = delete;
        Access(Access&& other) noexcept = default;
        Access& operator=(Access&& other) noexcept = default;

        ~Access() { _release(); }

        /**
         * Returns the metrics of the font.
         */
        const FontMetrics& get_metrics();

        /**
         * Retrieves the glyph corresponding to the specified Unicode code point
         * (returns NULL if not found).
         */
        const Glyph* find_glyph(cl7::text::codec::codepoint codepoint);

    private:
        Access(Font& font, FontLoader& font_loader);

        void _release();

        Font* _font;
        FontLoader* _font_loader;

        friend class Font;
    };



    explicit Font(std::unique_ptr<FontLoader> font_loader);



    /**
     * Gains access to retrieve metrics and glyphs of the font.
     */
    Access access();



private:
    /** The (possibly) loaded metrics of the font. */
    std::optional<FontMetrics> _metrics;

    /** Maps ASCII code points to their corresponding glyphs, along with a flag indicating whether the glyph has already been loaded. */
    std::array<std::pair<Glyph, bool>, 0x7e - 0x20 + 1> _ascii_glyphs;
    /** Maps non-ASCII Unicode code points to their respective glyphs (if they have already beed loaded). */
    std::unordered_map<cl7::text::codec::codepoint::value_type, Glyph> _non_ascii_glyph_map;

    /** The font loader for providing our font data. */
    std::unique_ptr<FontLoader> _font_loader;

}; // class Font



} // namespace fl7::fonts

#endif // FL7_FONTS_FONT_H
