#include "Unescaper.h"

#include <CoreLabs/text/format.h>
#include <CoreLabs/text/inspect.h>

#include <cstdint>



namespace dl7::yaml::util {



namespace {

    void _append_hex_escape(cl7::u8string& out, std::uint32_t value)
    {
        static constexpr cl7::u8char_t HEX_DIGITS[] = u8"0123456789ABCDEF";

        out += u8"\\u";
        out += HEX_DIGITS[(value >> 12) & 0xf];
        out += HEX_DIGITS[(value >> 8) & 0xf];
        out += HEX_DIGITS[(value >> 4) & 0xf];
        out += HEX_DIGITS[value & 0xf];
    }

    /**
     * Appends the given code point in the `\uXXXX` spelling JSON uses, resorting to
     * a surrogate pair beyond the basic multilingual plane. Returns false if the
     * code point is out of range.
     */
    bool _append_code_point_escape(cl7::u8string& out, std::uint32_t code_point)
    {
        if (code_point <= 0xffff)
        {
            _append_hex_escape(out, code_point);
            return true;
        }

        if (code_point > 0x10ffff)
            return false;

        code_point -= 0x10000;
        _append_hex_escape(out, 0xd800 + (code_point >> 10));
        _append_hex_escape(out, 0xdc00 + (code_point & 0x3ff));
        return true;
    }

} // namespace



    /**
     * Unescapes a double-quoted YAML scalar (excluding the surrounding quotes) and
     * returns the result as a new string.
     */
    cl7::u8string Unescaper::unescape_string(cl7::u8string_view yaml_string) const
    {
        const json::util::Unescaper unescaper{_error_handler};

        return unescaper.unescape_string(_translate_escapes(yaml_string));
    }



    /**
     * Rewrites the escape sequences YAML has beyond JSON's into their JSON
     * equivalents, leaving everything else (including `\\`) untouched.
     */
    cl7::u8string Unescaper::_translate_escapes(cl7::u8string_view yaml_string)
    {
        cl7::u8string result;
        result.reserve(yaml_string.size());

        for (size_t i = 0; i < yaml_string.size(); ++i)
        {
            const auto ch = yaml_string[i];

            if (ch != u8'\\' || i + 1 >= yaml_string.size())
            {
                result += ch;
                continue;
            }

            switch (yaml_string[i + 1])
            {
            case u8'0': result += u8'\0'; break;
            case u8'a': result += u8'\a'; break;
            case u8'e': result += static_cast<cl7::u8char_t>(0x1b); break;
            case u8' ': result += u8' '; break;
            case u8'N': result += u8"\u0085"; break; // next line
            case u8'_': result += u8"\u00a0"; break; // no-break space
            case u8'L': result += u8"\u2028"; break; // line separator
            case u8'P': result += u8"\u2029"; break; // paragraph separator

            case u8'U':
            {
                const auto digits = yaml_string.substr(i + 2);
                bool valid = digits.size() >= 8;
                for (size_t k = 0; valid && k < 8; ++k)
                    valid = cl7::text::inspect::is_hex_digit(digits[k]);

                if (valid && _append_code_point_escape(result, cl7::text::format::parse_hex<std::uint32_t>(digits.substr(0, 8))))
                {
                    i += 9;
                    continue;
                }

                // Not a `\UXXXXXXXX` sequence after all: leave it to the JSON
                // unescaper to complain about.
                result += ch;
                result += yaml_string[i + 1];
                break;
            }

            default:
                result += ch;
                result += yaml_string[i + 1];
                break;
            }

            ++i;
        }

        return result;
    }



} // namespace dl7::yaml::util
