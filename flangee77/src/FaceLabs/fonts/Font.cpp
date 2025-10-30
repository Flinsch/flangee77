#include "Font.h"



namespace fl7::fonts {



    Font::Access::Access(Font& font, FontLoader& font_loader)
        : _font(&font)
        , _font_loader(&font_loader)
    {
    }

    void Font::Access::_release()
    {
        _font_loader->idle();
    }




    /**
     * Returns the metrics of the font.
     */
    const FontMetrics& Font::Access::get_metrics()
    {
        if (!_font->_metrics.has_value())
            _font->_metrics = _font_loader->load_metrics();

        return *_font->_metrics;
    }



    /**
     * Retrieves the glyph corresponding to the specified Unicode code point
     * (returns NULL if not found).
     */
    const Glyph* Font::Access::find_glyph(cl7::text::codec::codepoint codepoint)
    {
        const size_t ascii_index = codepoint.value - 0x20;
        if (ascii_index < _font->_ascii_glyphs.size())
        {
            if (_font->_ascii_glyphs[ascii_index].second)
                return &_font->_ascii_glyphs[ascii_index].first;
        }
        else
        {
            auto it = _font->_non_ascii_glyph_map.find(codepoint.value);
            if (it != _font->_non_ascii_glyph_map.end())
                return &it->second;
        }

        if (!_font_loader->has_glyph(codepoint))
            return nullptr;

        Glyph glyph = _font_loader->load_glyph(codepoint);

        if (ascii_index < _font->_ascii_glyphs.size())
        {
            _font->_ascii_glyphs[ascii_index].first = std::move(glyph);
            _font->_ascii_glyphs[ascii_index].second = true;
            return &_font->_ascii_glyphs[ascii_index].first;
        }

        auto p = _font->_non_ascii_glyph_map.emplace(codepoint.value, std::move(glyph));
        auto& it = p.first;
        return &it->second;
    }



    Font::Font(std::unique_ptr<FontLoader> font_loader)
        : _font_loader(std::move(font_loader))
    {
        std::vector<uint32_t> glyph_indices;
        cl7::text::codec::codepoint::value_type from = 0x20;
        cl7::text::codec::codepoint::value_type to = 0x7e;
        auto access = this->access();
        for (auto cpv = from; cpv <= to; ++cpv)
            access.find_glyph({cpv});
    }



    /**
     * Gains access to retrieve metrics and glyphs of the font.
     */
    Font::Access Font::access()
    {
        return {*this, *_font_loader};
    }



} // namespace fl7::fonts
