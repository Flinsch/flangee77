#include "Escaper.h"

#include <CoreLabs/strings/codec/Encoder.h>
#include <CoreLabs/strings/codec/Decoder.h>

#include <format>



namespace dl7::json::util {



    /**
     * Escapes a raw string for inclusion in a JSON string literal and returns the
     * result as a new string.
     */
    cl7::u8string Escaper::escape_string(cl7::u8string_view raw_string, const Options& options) const
    {
        cl7::u8osstream out;
        escape_string(out, raw_string, options);
        return out.str();
    }

    /**
     * Escapes a raw string for inclusion in a JSON string literal and writes the
     * result to the provided output stream.
     */
    void Escaper::escape_string(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options) const
    {
        if (options.escape_unicode)
            _do_escape_utf16_format(out, raw_string, options);
        else
            _do_escape_utf8_encoded(out, raw_string, options);
    }



    void Escaper::_do_escape_utf8_encoded(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options) const
    {
        cl7::strings::codec::DefaultErrorHandler encoding_error_handler{_log_context};

        cl7::strings::codec::Encoder<cl7::u8char_t> utf8_encoder{&encoding_error_handler};
        cl7::u8char_t utf8_buffer[4];

        cl7::strings::codec::Decoder<cl7::u8char_t>::iterator it{raw_string, &encoding_error_handler};
        cl7::strings::codec::Decoder<cl7::u8char_t>::sentinel end;

        for (; it != end; ++it)
        {
            const auto codepoint = *it;

            if (_try_escape_basic_helper(out, codepoint.value, options))
                continue;

            // Encode code point "regular" as UTF-8.
            const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
            out << cl7::u8string_view(utf8_buffer, size);

        } // for each Unicode code point
    }

    void Escaper::_do_escape_utf16_format(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options) const
    {
        cl7::strings::codec::DefaultErrorHandler encoding_error_handler{_log_context};

        cl7::strings::codec::Encoder<cl7::u16char_t> utf16_encoder{&encoding_error_handler};
        cl7::u16char_t utf16_buffer[2];

        cl7::strings::codec::Decoder<cl7::u8char_t>::iterator it{raw_string, &encoding_error_handler};
        cl7::strings::codec::Decoder<cl7::u8char_t>::sentinel end;

        for (; it != end; ++it)
        {
            const auto codepoint = *it;

            if (_try_escape_basic_helper(out, codepoint.value, options))
                continue;

            // Escape code point using UTF-16 format, `\uXXXX`.
            const auto size = utf16_encoder.encode_one_into(codepoint, cl7::u16string_span(utf16_buffer, 2));
            out << std::format("\\u{:04X}", static_cast<unsigned>(utf16_buffer[0]));
            if (size > 1)
                out << std::format("\\u{:04X}", static_cast<unsigned>(utf16_buffer[1]));

        } // for each Unicode code point
    }

    bool Escaper::_try_escape_basic_helper(cl7::u8osstream& out, unsigned codepoint_value, const Options& options)
    {
        const auto value = codepoint_value;

        switch (value)
        {
            // Backslash-escape backspace, tab, newline,
            // form feed, and carriage return.
        case u8'\b': out << u8"\\b"; return true;
        case u8'\t': out << u8"\\t"; return true;
        case u8'\n': out << u8"\\n"; return true;
        case u8'\f': out << u8"\\f"; return true;
        case u8'\r': out << u8"\\r"; return true;

            // Handle single and/or double quotes.
        case u8'"':
        case u8'\'':
            if (value == static_cast<cl7::u8char_t>(options.quote_char))
                out << u8'\\';
            out << static_cast<cl7::u8char_t>(value);
            return true;

            // Of course, "self-escape" backslash as well.
        case u8'\\': out << u8"\\\\"; return true;

            // We have to handle range-based
            // cases using if statements.
        default:

            // Escape (other) control characters.
            if (value < 0x20)
            {
                out << std::format("\\u{:04X}", value);
                return true;
            }

            // ASCII can be simply "waved through".
            if (value < 0x80)
            {
                out << static_cast<cl7::u8char_t>(value);
                return true;
            }

        } // switch code point

        return false; // Non-ASCII not handled.
    }



} // namespace dl7::json::util
