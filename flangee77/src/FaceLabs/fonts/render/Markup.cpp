#include "Markup.h"

#include <CoreLabs/text/codec/Encoder.h>

#include <charconv>
#include <string>
#include <string_view>



namespace fl7::fonts::render {



namespace {

    enum class TagKind { B, I, Color };

    struct OpenTag
    {
        TagKind kind;
        GlyphStyle style;
    };

    bool _is_tag_char(cl7::text::codec::codepoint cp)
    {
        const auto v = cp.value;
        return (v >= 'a' && v <= 'z') || (v >= 'A' && v <= 'Z') || (v >= '0' && v <= '9')
            || v == '/' || v == '=' || v == '#' || v == '.' || v == '-' || v == '_';
    }

    // Converts a span of code points already known to satisfy _is_tag_char
    // (i.e., plain ASCII) into an std::string for parsing.
    std::string _to_ascii(std::span<const cl7::text::codec::codepoint> codepoints)
    {
        std::string s;
        s.reserve(codepoints.size());
        for (const auto& cp : codepoints)
            s.push_back(static_cast<char>(cp.value));
        return s;
    }

    bool _parse_float(std::string_view s, float& out)
    {
        const auto result = std::from_chars(s.data(), s.data() + s.size(), out);
        return result.ec == std::errc{} && result.ptr == s.data() + s.size();
    }

    bool _parse_hex_digit(char c, unsigned& out)
    {
        if (c >= '0' && c <= '9') { out = static_cast<unsigned>(c - '0'); return true; }
        if (c >= 'a' && c <= 'f') { out = static_cast<unsigned>(c - 'a' + 10); return true; }
        if (c >= 'A' && c <= 'F') { out = static_cast<unsigned>(c - 'A' + 10); return true; }
        return false;
    }

    bool _parse_hex_byte(std::string_view s, size_t offset, uint8_t& out)
    {
        unsigned hi = 0;
        unsigned lo = 0;
        if (!_parse_hex_digit(s[offset], hi) || !_parse_hex_digit(s[offset + 1], lo))
            return false;
        out = static_cast<uint8_t>((hi << 4u) | lo);
        return true;
    }

    // #RRGGBB or #RRGGBBAA.
    bool _parse_color(std::string_view s, xl7::graphics::Color& out)
    {
        if ((s.size() != 7 && s.size() != 9) || s[0] != '#')
            return false;

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0xff;
        if (!_parse_hex_byte(s, 1, r) || !_parse_hex_byte(s, 3, g) || !_parse_hex_byte(s, 5, b))
            return false;
        if (s.size() == 9 && !_parse_hex_byte(s, 7, a))
            return false;

        out = xl7::graphics::Color(r, g, b, a);
        return true;
    }

    // `name` is plain ASCII (constrained by _is_tag_char), compared byte-wise
    // against a (possibly non-ASCII, though unlikely for icon names) UTF-8
    // registry name.
    bool _ascii_equals_u8(std::string_view name, cl7::u8string_view u8_name)
    {
        if (name.size() != u8_name.size())
            return false;
        for (size_t k = 0; k < name.size(); ++k)
        {
            if (static_cast<unsigned char>(name[k]) != static_cast<unsigned char>(u8_name[k]))
                return false;
        }
        return true;
    }

    const Icon* _find_named_icon(std::string_view name, std::span<const NamedIcon> icons)
    {
        for (const NamedIcon& named : icons)
        {
            if (_ascii_equals_u8(name, named.name))
                return named.icon;
        }
        return nullptr;
    }

} // namespace



    ParsedMarkup _parse_markup(std::span<const cl7::text::codec::codepoint> codepoints, const TextStyle& base_style, std::span<const NamedIcon> icons)
    {
        const GlyphStyle base_glyph_style = base_style; // NOLINT(cppcoreguidelines-slicing)

        ParsedMarkup result;
        std::vector<cl7::text::codec::codepoint> output;
        output.reserve(codepoints.size());

        std::vector<OpenTag> stack;
        size_t interval_start = 0;

        // Closes off the interval [interval_start, end_index) with a StyleRun
        // for whatever style is currently on top of the stack, unless there is
        // none (i.e., the base style already applies with no override needed).
        const auto close_interval = [&](size_t end_index)
        {
            if (!stack.empty() && end_index > interval_start)
                result.style_runs.push_back(StyleRun{.codepoint_begin = interval_start, .codepoint_end = end_index, .style = stack.back().style});
            interval_start = end_index;
        };

        size_t i = 0;
        while (i < codepoints.size())
        {
            if (codepoints[i].value != '[')
            {
                output.push_back(codepoints[i]);
                ++i;
                continue;
            }

            size_t j = i + 1;
            while (j < codepoints.size() && _is_tag_char(codepoints[j]))
                ++j;

            if (j >= codepoints.size() || codepoints[j].value != ']')
            {
                // No well-formed "[...]" group here: the '[' is just literal text.
                output.push_back(codepoints[i]);
                ++i;
                continue;
            }

            const std::span<const cl7::text::codec::codepoint> inner = codepoints.subspan(i + 1, j - i - 1);
            const std::string content = _to_ascii(inner);
            const std::span<const cl7::text::codec::codepoint> whole_tag = codepoints.subspan(i, j - i + 1);

            bool recognized = false;

            if (!content.empty() && content.front() == '/')
            {
                const std::string_view kind_str = std::string_view(content).substr(1);
                if (kind_str == "b" || kind_str == "i" || kind_str == "color")
                {
                    TagKind kind = TagKind::Color;
                    if (kind_str == "b")
                        kind = TagKind::B;
                    else if (kind_str == "i")
                        kind = TagKind::I;

                    if (!stack.empty() && stack.back().kind == kind)
                    {
                        close_interval(output.size());
                        stack.pop_back();
                        recognized = true;
                    }
                }
            }
            else
            {
                const size_t eq = content.find('=');
                const std::string_view kind_str = eq == std::string::npos ? std::string_view(content) : std::string_view(content).substr(0, eq);
                const std::string_view value_str = eq == std::string::npos ? std::string_view{} : std::string_view(content).substr(eq + 1);

                if (kind_str == "b" || kind_str == "i")
                {
                    float value = 1.0f;
                    if (value_str.empty() || _parse_float(value_str, value))
                    {
                        close_interval(output.size());
                        GlyphStyle style = stack.empty() ? base_glyph_style : stack.back().style;
                        if (kind_str == "b")
                            style.weight = value;
                        else
                            style.italic_intensity = value;
                        stack.push_back(OpenTag{.kind = kind_str == "b" ? TagKind::B : TagKind::I, .style = style});
                        recognized = true;
                    }
                }
                else if (kind_str == "color")
                {
                    xl7::graphics::Color color;
                    if (!value_str.empty() && _parse_color(value_str, color))
                    {
                        close_interval(output.size());
                        GlyphStyle style = stack.empty() ? base_glyph_style : stack.back().style;
                        style.text_color = color;
                        stack.push_back(OpenTag{.kind = TagKind::Color, .style = style});
                        recognized = true;
                    }
                }
                else if (kind_str == "icon")
                {
                    const Icon* icon = value_str.empty() ? nullptr : _find_named_icon(value_str, icons);
                    if (icon)
                    {
                        output.push_back(cl7::text::codec::codepoint{0xFFFC});
                        result.icon_runs.push_back(IconRun{.codepoint_index = output.size() - 1, .icon = icon});
                        recognized = true;
                    }
                }
            }

            if (!recognized)
                output.insert(output.end(), whole_tag.begin(), whole_tag.end());

            i = j + 1;
        } // for each code point and/or tag

        // Any tags still open at this point are treated as implicitly
        // closed at the end of the text (not an error).
        close_interval(output.size());

        cl7::text::codec::Encoder<cl7::u8char_t> encoder;
        result.text.clear();
        result.text.reserve(output.size());
        for (const cl7::text::codec::codepoint& cp : output)
            result.text += encoder.encode_one(cp);

        return result;
    }



} // namespace fl7::fonts::render
