#include "JsonWriter.h"

#include <CoreLabs/strings/Encoder.h>
#include <CoreLabs/strings/Decoder.h>
#include <CoreLabs/sstream.h>

#include <cmath>
#include <format>
#include <unordered_set>



namespace dl7::json {



    static void _put_line_ending(cl7::u8osstream& oss, const JsonWriter::Format& format)
    {
        assert(format.style == JsonWriter::Format::Style::MultiLine);

        switch (format.multi_line_options.line_ending)
        {
        case JsonWriter::Format::MultiLineOptions::LineEnding::CRLF:
            oss << "\r\n";
            break;
        case JsonWriter::Format::MultiLineOptions::LineEnding::LF:
            oss << '\n';
            break;
        case JsonWriter::Format::MultiLineOptions::LineEnding::CR:
            oss << '\r';
            break;
        }
    }

    static void _start_item(cl7::u8osstream& oss, size_t depth, const JsonWriter::Format& format)
    {
        if (format.style != JsonWriter::Format::Style::MultiLine)
            return;

        _put_line_ending(oss, format);

        if (format.multi_line_options.indentation == JsonWriter::Format::MultiLineOptions::Indentation::Tabs)
            for (size_t i = 0; i < depth; ++i)
                oss.put(u8'\t');
        else // => spaces
            for (size_t i = 0; i < depth * static_cast<unsigned>(format.multi_line_options.indentation); ++i)
                oss.put(u8' ');
    }

    static void _end_item(cl7::u8osstream& oss, size_t index, size_t count, const JsonWriter::Format& format)
    {
        assert(index < count);
        const bool is_last = index == count - 1;

        if (is_last)
        {
            if (format.style == JsonWriter::Format::Style::MultiLine && format.multi_line_options.add_trailing_commas)
                oss.put(u8',');
        }
        else // => !is_last
        {
            oss.put(u8',');
            if (format.style == JsonWriter::Format::Style::SingleLine && !format.single_line_options.compact)
                oss.put(u8' ');
        }
    }

    static cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_t quote_char, const JsonWriter::Format& format)
    {
        oss << quote_char;

        cl7::strings::Encoder<cl7::u8char_t> utf8_encoder;
        cl7::u8char_t utf8_buffer[4];

        cl7::strings::Decoder<cl7::u8char_t>::iterator it{string};
        cl7::strings::Decoder<cl7::u8char_t>::sentinel end;

        for (; it != end; ++it)
        {
            const auto codepoint = *it;
            const auto value = codepoint.value;

            switch (value)
            {
                // Backslash-escape backspace, tab, newline,
                // form feed, and carriage return.
            case u8'\b': oss << u8"\\b"; break;
            case u8'\t': oss << u8"\\t"; break;
            case u8'\n': oss << u8"\\n"; break;
            case u8'\f': oss << u8"\\f"; break;
            case u8'\r': oss << u8"\\r"; break;

                // Handle single and/or double quotes.
            case u8'"':
            case u8'\'':
                if (value == quote_char)
                    oss << u8'\\';
                oss << static_cast<cl7::u8char_t>(value);
                break;

                // Of course, "self-escape" backslash as well.
            case u8'\\': oss << u8"\\\\"; break;

                // Okay, the actual case distinction
                // is just beginning right now.
            default:
                if (value < 0x20)
                    // Escape control characters.
                    oss << std::format("\\u{:04X}", value);

                else if (value < 0x80)
                    // ASCII can be simply "waved through".
                    oss << static_cast<cl7::u8char_t>(value);

                else if (format.escape_unicode)
                {
                    if (value <= 0xffff)
                        // Escape code point within the Basic
                        // Multilingual Plane (U+0000 to U+FFFF).
                        oss << std::format("\\u{:04X}", value);

                    else
                    {
                        // Escape code point outside the Basic Multilingual Plane
                        // (above U+FFFF) using UTF-16 surrogate pair.
                        cl7::strings::Encoder<cl7::u16char_t> utf16_encoder;
                        cl7::u16char_t utf16_buffer[2];

                        const auto size = utf16_encoder.encode_one_into(codepoint, cl7::u16string_span(utf16_buffer, 2));
                        assert(size == 2);

                        oss << std::format("\\u{:04X}", static_cast<unsigned>(utf16_buffer[0]));
                        oss << std::format("\\u{:04X}", static_cast<unsigned>(utf16_buffer[1]));
                    }
                }
                else
                {
                    // Encode code point "regular" as UTF-8.
                    const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                    oss << cl7::u8string_view(utf8_buffer, size);
                }
            } // switch code point
        } // for each Unicode code point

        oss << quote_char;
        return oss;
    }

    static cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, const JsonWriter::Format& format)
    {
        if (!format.allow_single_quotes)
            return _write_string(oss, string, u8'"', format);

        size_t single_quotes = 0;
        size_t double_quotes = 0;
        for (const auto& ch : string)
        {
            if (ch == u8'\'') ++single_quotes;
            if (ch == u8'"') ++double_quotes;
        }

        cl7::u8char_t quote_char = single_quotes > double_quotes ? u8'\'' : u8'"';
        return _write_string(oss, string, quote_char, format);
    }

    static cl7::u8osstream& _write_decimal(cl7::u8osstream& oss, decimal_t decimal, const JsonWriter::Format& format)
    {
        decimal_t int_part;
        decimal_t frac_part = std::modf(decimal, &int_part);
        if (frac_part == decimal_t{0.0})
        {
            constexpr decimal_t min_int = std::numeric_limits<signed long>::min();
            constexpr decimal_t max_int = std::numeric_limits<signed long>::max();
            if (int_part >= min_int && int_part <= max_int)
                return oss << static_cast<signed long>(int_part) << u8".0";
            return oss << std::scientific << int_part;
        }

        return oss << std::defaultfloat << decimal;
    }

    static cl7::u8osstream& _write_json(cl7::u8osstream& oss, const Json& json, size_t depth, const JsonWriter::Format& format)
    {
        if (json.is_null()) return oss << u8"null";
        if (json.is_true()) return oss << u8"true";
        if (json.is_false()) return oss << u8"false";
        if (json.is_string()) return _write_string(oss, json.as_string(), format);
        if (json.is_integer()) return oss << json.as_integer();
        if (json.is_unsigned()) return oss << json.as_unsigned();
        if (json.is_decimal()) return _write_decimal(oss, json.as_decimal(), format);

        if (json.is_array())
        {
            const array_t& array = json.as_array();
            if (array.empty()) return oss << u8"[]";
            oss << u8"[";
            for (size_t i = 0; i < array.size(); ++i)
            {
                _start_item(oss, depth + 1, format);
                _write_json(oss, array[i], depth + 1, format);
                _end_item(oss, i, array.size(), format);
            }
            _start_item(oss, depth, format);
            oss << u8"]";
            return oss;
        }

        if (json.is_object())
        {
            const object_t& object = json.as_object();
            if (object.empty()) return oss << u8"{}";
            oss << u8"[";
            size_t i = 0;
            for (const auto& p : object)
            {
                _start_item(oss, depth + 1, format);
                if (format.allow_unquoted_keys && JsonWriter::is_valid_unquoted_key(p.first))
                    oss << p.first;
                else
                    _write_string(oss, p.first, format);
                oss.put(u8':');
                if (format.style == JsonWriter::Format::Style::SingleLine && !format.single_line_options.compact)
                    oss.put(u8' ');
                _write_json(oss, p.second, depth + 1, format);
                _end_item(oss, i, object.size(), format);
                ++i;
            }
            _start_item(oss, depth, format);
            oss << u8"]";
            return oss;
        }

        assert(false);
        return oss;
    }



    /**
     * Generates a string representing the given JSON object/value based on the
     * specified format.
     */
    cl7::u8string JsonWriter::to_string(const Json& json, const Format& format)
    {
        cl7::u8osstream oss;

        _write_json(oss, json, 0, format);

        if (format.style == JsonWriter::Format::Style::MultiLine && format.multi_line_options.add_empty_line && json.is_structured() && !json.is_empty())
            _put_line_ending(oss, format);

        return std::move(oss).str();
    }



    /**
     * Checks whether the specified string is a valid identifier for an unquoted
     * object key. Not all conditions of JavaScript's identifier naming rules are
     * considered, as with JSON5, for example. We allow identifiers that begin with
     * a letter (A-Z or a-z), an underscore, or a dollar sign; consist of letters,
     * digits, underscores, and/or dollar signs; and do not correspond to any
     * reserved keyword (as per the ECMAScript specification).
     */
    bool JsonWriter::is_valid_unquoted_key(cl7::u8string_view key)
    {
        // Empty keys are invalid.
        if (key.empty())
            return false;

        // First character must be a letter (A-Z, a-z), underscore (_), or dollar sign ($).
        if (!(std::isalpha(key[0]) || key[0] == '_' || key[0] == '$'))
            return false;

        // Remaining characters can be letters, digits, underscores, or dollar signs.
        for (size_t i = 1; i < key.size(); ++i)
        {
            if (!(std::isalnum(key[i]) || key[i] == '_' || key[i] == '$'))
                return false;
        }

        // JavaScript reserved words:
        // https://tc39.es/ecma262/#prod-ReservedWord
        const std::unordered_set<cl7::u8string_view> JS_RESERVED_WORDS = {
            u8"await", u8"break", u8"case", u8"catch", u8"class", u8"const", u8"continue", u8"debugger", u8"default",
            u8"delete", u8"do", u8"else", u8"enum", u8"export", u8"extends", u8"false", u8"finally", u8"for",
            u8"function", u8"if", u8"import", u8"in", u8"instanceof", u8"new", u8"null", u8"return", u8"super",
            u8"switch", u8"this", u8"throw", u8"true", u8"try", u8"typeof", u8"var", u8"void", u8"while", u8"with",
            u8"yield",
        };

        // Ensure the key is not a reserved JavaScript keyword.
        return !JS_RESERVED_WORDS.contains(key);
    }



} // namespace dl7::json
