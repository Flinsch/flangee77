#include "Font.h"



namespace fl7::fonts {



    Font::Font(std::unique_ptr<FontLoader> font_loader)
        : _font_loader(std::move(font_loader))
    {
    }



    /**
     * Returns the metrics of the font.
     */
    const FontMetrics& Font::get_metrics()
    {
        if (!_metrics.has_value())
            _metrics = _font_loader->load_metrics();

        return *_metrics;
    }



    /**
     * Retrieves the glyph corresponding to the specified Unicode code point
     * (returns NULL if not found).
     */
    const Glyph* Font::find_glyph(cl7::text::codec::codepoint codepoint)
    {
        const size_t ascii_index = codepoint.value - 0x20;
        if (ascii_index < _ascii_glyphs.size())
        {
            if (_ascii_glyphs[ascii_index].second)
                return &_ascii_glyphs[ascii_index].first;
        }
        else
        {
            auto it = _non_ascii_glyph_map.find(codepoint.value);
            if (it != _non_ascii_glyph_map.end())
                return &it->second;
        }

        if (!_font_loader->has_glyph(codepoint))
            return nullptr;

        Glyph glyph = _font_loader->load_glyph(codepoint);

        if (ascii_index < _ascii_glyphs.size())
        {
            _ascii_glyphs[ascii_index].first = std::move(glyph);
            _ascii_glyphs[ascii_index].second = true;
            return &_ascii_glyphs[ascii_index].first;
        }

        auto p = _non_ascii_glyph_map.emplace(codepoint.value, std::move(glyph));
        auto& it = p.first;
        return &it->second;
    }



} // namespace fl7::fonts
