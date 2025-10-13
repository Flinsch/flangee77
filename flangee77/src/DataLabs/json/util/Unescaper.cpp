#include "Unescaper.h"

#include <CoreLabs/text/codec/Encoder.h>
#include <CoreLabs/text/codec/Decoder.h>
#include <CoreLabs/text/format.h>
#include <CoreLabs/text/inspect.h>

#include <format>



namespace dl7::json::util {



    /**
     * Unescapes a JSON string (excluding surrounding quotes) and returns the result
     * as a new string. Returns an empty string on invalid escape sequences or
     * invalid UTF-8.
     */
    cl7::u8string Unescaper::unescape_string(cl7::u8string_view json_string) const
    {
        cl7::u8osstream out;
        unescape_string(out, json_string);
        return out.str();
    }

    /**
     * Unescapes a JSON string (excluding surrounding quotes) and writes the result
     * to the provided output stream. Returns false on invalid escape sequences or
     * invalid UTF-8 (returns true on success).
     */
    bool Unescaper::unescape_string(cl7::u8osstream& out, cl7::u8string_view json_string) const
    {
        ErrorHandler default_error_handler;
        const ErrorHandler* my_error_handler = _error_handler ? _error_handler : &default_error_handler;
        unsigned error_count = 0;

        struct EncodingErrorHandler
            : public cl7::text::codec::DefaultErrorHandler
        {
            const Unescaper::ErrorHandler* _error_handler;
            unsigned* _error_count;

            EncodingErrorHandler(const Unescaper::ErrorHandler* error_handler, unsigned* error_count)
                : cl7::text::codec::DefaultErrorHandler(error_handler->log_context)
                , _error_handler(error_handler)
                , _error_count(error_count)
            {
            }

        protected:
            void _on_any_error(cl7::text::codec::EncodingError error) const override
            {
                ++*_error_count;
            }

            void _on_decode_error(cl7::text::codec::EncodingError error, cl7::u8string_view input_read) const override
            {
                _error_handler->on_invalid_utf8_encoding(input_read);
            }

            void _on_decode_error(cl7::text::codec::EncodingError error, cl7::u16string_view input_read) const override
            {
                _error_handler->on_invalid_utf16_encoding(input_read);
            }
        } encoding_error_handler{my_error_handler, &error_count};

        cl7::text::codec::Encoder<cl7::u8char_t> utf8_encoder{&encoding_error_handler};
        cl7::u8char_t utf8_buffer[4];

        cl7::text::codec::Decoder<cl7::u16char_t> utf16_decoder{&encoding_error_handler};
        cl7::u16char_t utf16_buffer[2];

        cl7::text::codec::Decoder<cl7::u8char_t>::iterator it{json_string, &encoding_error_handler};
        cl7::text::codec::Decoder<cl7::u8char_t>::iterator end;

        for (; it != end; ++it)
        {
            auto pos = it.pos();
            auto codepoint = *it;

            if (codepoint.value != u8'\\')
            {
                const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                out << cl7::u8string_view(utf8_buffer, size);
                continue;
            }

            if (++it == end) // Skip backslash.
            {
                // Unexpected end of string.
                out << my_error_handler->handle_invalid_escape_sequence(json_string.substr(pos));
                return false;
            }

            codepoint = *it;

            switch (codepoint.value)
            {
                // Handle backspace, tab, newline,
                // form feed, and carriage return.
            case u8'b': out.put(u8'\b'); break;
            case u8't': out.put(u8'\t'); break;
            case u8'n': out.put(u8'\n'); break;
            case u8'f': out.put(u8'\f'); break;
            case u8'r': out.put(u8'\r'); break;

                // Handle single and/or double quotes
                // (without warning for "wrong" quotes).
            case u8'"': out.put(u8'"'); break;
            case u8'\'': out.put(u8'\''); break;

                // Handle backslash.
            case u8'\\': out.put(u8'\\'); break;

                // Be forgiving: handle `\v` and `/` without error/warning.
            case u8'v': out.put(u8'\v'); break;
            case u8'/': out.put(u8'/'); break;

                // (Try to) handle `\uXXXX`.
            case u8'u':
            {
                if (pos + 6 > json_string.size() || !cl7::text::inspect::is_hex_digit(json_string[pos + 2]) || !cl7::text::inspect::is_hex_digit(json_string[pos + 3]) || !cl7::text::inspect::is_hex_digit(json_string[pos + 4]) || !cl7::text::inspect::is_hex_digit(json_string[pos + 5]))
                {
                    // Invalid `\uXXXX` sequence.
                    size_t count = 0;
                    while (pos + count + 2 < json_string.size() && cl7::text::inspect::is_hex_digit(json_string[pos + count + 2]))
                        ++count;
                    assert(count < 4);

                    out << my_error_handler->handle_invalid_escape_sequence(json_string.substr(pos, count + 2));
                    ++error_count;

                    it.reset_pos(pos + count + 2 - 1);
                    break;
                }

                codepoint.value = cl7::text::format::parse_hex(json_string.substr(pos + 2, 4));
                pos += 6;

                // Check for surrogate pair.
                if (codepoint.is_high_surrogate() && pos + 6 <= json_string.size() && json_string[pos + 0] == u8'\\' && json_string[pos + 1] == u8'u' && cl7::text::inspect::is_hex_digit(json_string[pos + 2]) && cl7::text::inspect::is_hex_digit(json_string[pos + 3]) && cl7::text::inspect::is_hex_digit(json_string[pos + 4]) && cl7::text::inspect::is_hex_digit(json_string[pos + 5]))
                {
                    auto second_codepoint = cl7::text::codec::codepoint{cl7::text::format::parse_hex(json_string.substr(pos + 2, 4))};

                    if (second_codepoint.is_low_surrogate()) // Play it safe!
                    {
                        utf16_buffer[0] = static_cast<cl7::u16char_t>(codepoint.value);
                        utf16_buffer[1] = static_cast<cl7::u16char_t>(second_codepoint.value);
                        codepoint = utf16_decoder.decode_one(cl7::u16string_view{utf16_buffer, 2});
                        pos += 6;
                    }
                }

                const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                out << cl7::u8string_view(utf8_buffer, size);

                it.reset_pos(pos - 1);
                break;
            }
                // (Try to) be forgiving again: handle `\xXX` as well.
            case u8'x':
            {
                if (pos + 4 > json_string.size() || !cl7::text::inspect::is_hex_digit(json_string[pos + 2]) || !cl7::text::inspect::is_hex_digit(json_string[pos + 3]))
                {
                    // Invalid `\xXX` sequence.
                    size_t count = 0;
                    while (pos + count + 2 < json_string.size() && cl7::text::inspect::is_hex_digit(json_string[pos + count + 2]))
                        ++count;
                    assert(count < 2);

                    out << my_error_handler->handle_invalid_escape_sequence(json_string.substr(pos, count + 2));
                    ++error_count;

                    it.reset_pos(pos + count + 2 - 1);
                    break;
                }

                cl7::u8osstream dummy_oss;
                dummy_oss << cl7::text::format::parse_hex<cl7::u8char_t>(json_string.substr(pos + 2, 2));
                pos += 4;

                // Look for more of those `\xXX` and concatenate them all
                // to capture any UTF-8 sequences encoded in this way.
                while (pos + 4 <= json_string.size() && json_string[pos + 0] == u8'\\' && json_string[pos + 1] == u8'x' && cl7::text::inspect::is_hex_digit(json_string[pos + 2]) && cl7::text::inspect::is_hex_digit(json_string[pos + 3]))
                {
                    dummy_oss << cl7::text::format::parse_hex<cl7::u8char_t>(json_string.substr(pos + 2, 2));
                    pos += 4;
                }

                const auto dummy_string = dummy_oss.str();
                cl7::text::codec::Decoder<cl7::u8char_t>::iterator dummy_it{dummy_string, &encoding_error_handler};
                for (; dummy_it != end; ++dummy_it)
                {
                    codepoint = *dummy_it;

                    const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                    out << cl7::u8string_view(utf8_buffer, size);
                }

                it.reset_pos(pos - 1);
                break;
            }

                // Otherwise?
            default:
            {
                // Invalid escape sequence.
                out << my_error_handler->handle_invalid_escape_sequence(json_string.substr(pos, 2));
                ++error_count;
            }

            } // switch code point
        } // for each Unicode code point

        return error_count == 0;
    }



} // namespace dl7::json::util
