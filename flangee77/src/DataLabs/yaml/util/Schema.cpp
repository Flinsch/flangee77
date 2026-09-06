#include "Schema.h"

#include <CoreLabs/text/codec.h>

#include <charconv>
#include <cstdint>
#include <limits>
#include <optional>



namespace dl7::yaml::util {



namespace {

    bool _is_digit(cl7::u8char_t ch) { return ch >= u8'0' && ch <= u8'9'; }
    bool _is_octal_digit(cl7::u8char_t ch) { return ch >= u8'0' && ch <= u8'7'; }
    bool _is_hex_digit(cl7::u8char_t ch) { return _is_digit(ch) || (ch >= u8'a' && ch <= u8'f') || (ch >= u8'A' && ch <= u8'F'); }

    /**
     * Tries to parse an integer scalar according to the core schema, i.e., a
     * decimal, `0o`-prefixed octal, or `0x`-prefixed hexadecimal literal. Returns
     * std::nullopt if the given text is not such a literal (or does not fit).
     */
    std::optional<integer_t> _try_parse_integer(cl7::u8string_view text)
    {
        if (text.empty())
            return std::nullopt;

        size_t i = 0;
        bool negative = false;
        if (text[i] == u8'+' || text[i] == u8'-')
        {
            negative = text[i] == u8'-';
            ++i;
        }

        int base = 10;
        if (text.size() - i > 2 && text[i] == u8'0')
        {
            if (text[i + 1] == u8'o') { base = 8; i += 2; }
            else if (text[i + 1] == u8'x') { base = 16; i += 2; }
        }

        const auto digits = text.substr(i);
        if (digits.empty())
            return std::nullopt;

        for (const auto ch : digits)
        {
            const bool ok = base == 8 ? _is_octal_digit(ch) : (base == 16 ? _is_hex_digit(ch) : _is_digit(ch));
            if (!ok)
                return std::nullopt;
        }

        const std::string_view chars{cl7::text::codec::reinterpret_utf8(digits)};

        std::uint64_t value = 0;
        const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), value, base);
        if (ec != std::errc{} || ptr != chars.data() + chars.size())
            return std::nullopt;

        constexpr auto limit = static_cast<std::uint64_t>(std::numeric_limits<integer_t>::max());
        if (value > (negative ? limit + 1 : limit))
            return std::nullopt; // Out of range: let it be a float (or a string).

        if (negative)
            return static_cast<integer_t>(~value + 1);
        return static_cast<integer_t>(value);
    }

    /**
     * Tries to parse a floating-point scalar according to the core schema,
     * including `.inf` and `.nan` in their accepted spellings. Returns std::nullopt
     * if the given text is not such a literal.
     */
    std::optional<float_t> _try_parse_float(cl7::u8string_view text)
    {
        if (text.empty())
            return std::nullopt;

        size_t i = 0;
        bool negative = false;
        if (text[i] == u8'+' || text[i] == u8'-')
        {
            negative = text[i] == u8'-';
            ++i;
        }

        const auto rest = text.substr(i);

        if (rest == u8".inf" || rest == u8".Inf" || rest == u8".INF")
            return negative ? -std::numeric_limits<float_t>::infinity() : std::numeric_limits<float_t>::infinity();
        if (rest == u8".nan" || rest == u8".NaN" || rest == u8".NAN")
            return std::numeric_limits<float_t>::quiet_NaN();

        // Reject anything `std::from_chars` would accept but the core schema does
        // not (`inf`, `nan`, hexadecimal significands), i.e., insist on a leading
        // digit or decimal point.
        if (rest.empty() || (!_is_digit(rest[0]) && rest[0] != u8'.'))
            return std::nullopt;

        const std::string_view chars{cl7::text::codec::reinterpret_utf8(text)};

        float_t value = 0.0;
        const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), value, std::chars_format::general);
        if (ec != std::errc{} || ptr != chars.data() + chars.size())
            return std::nullopt;

        return value;
    }

} // namespace



    /**
     * Resolves a plain scalar into the value it stands for: null, a boolean, an
     * integer, a floating-point number, or (when failing all of those) the string
     * itself.
     */
    Yaml Schema::resolve(cl7::u8string_view text)
    {
        if (text.empty() || text == u8"~" || text == u8"null" || text == u8"Null" || text == u8"NULL")
            return {};

        if (text == u8"true" || text == u8"True" || text == u8"TRUE")
            return Yaml{true};
        if (text == u8"false" || text == u8"False" || text == u8"FALSE")
            return Yaml{false};

        if (const auto integer = _try_parse_integer(text))
            return Yaml{*integer};
        if (const auto number = _try_parse_float(text))
            return Yaml{*number};

        return Yaml{string_t{text}};
    }

    /**
     * Returns true if the given text resolves to a string of the very same content,
     * i.e., writing that string as a plain scalar would not change its type.
     */
    bool Schema::resolves_to_string(cl7::u8string_view text)
    {
        return resolve(text).is_string();
    }



} // namespace dl7::yaml::util
