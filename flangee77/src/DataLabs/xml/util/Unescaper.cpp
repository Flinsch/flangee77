#include "Unescaper.h"

#include <CoreLabs/text/codec/Encoder.h>
#include <CoreLabs/text/codec/Decoder.h>

#include "CoreLabs/text/format.h"
#include "CoreLabs/text/inspect.h"



namespace dl7::xml::util {



    /**
     * Unescapes an XML-escaped string literal (excluding any surrounding quotes)
     * and returns the result as a new string.
     */
    cl7::u8string Unescaper::unescape_string(cl7::u8string_view xml_string, UnescapeType unescape_type) const
    {
        cl7::u8osstream out;
        unescape_string(out, xml_string, unescape_type);
        return out.str();
    }

    /**
     * Unescapes an XML-escaped string literal (excluding any surrounding quotes)
     * and writes the result to the provided output stream. Returns false on invalid
     * escape sequences or invalid UTF-8 (returns true on success).
     */
    bool Unescaper::unescape_string(cl7::u8osstream& out, cl7::u8string_view xml_string, UnescapeType unescape_type) const
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
        } encoding_error_handler{my_error_handler, &error_count};

        cl7::text::codec::Encoder<cl7::u8char_t> utf8_encoder{&encoding_error_handler};
        cl7::u8char_t utf8_buffer[4];

        cl7::text::codec::Decoder<cl7::u8char_t>::iterator it{xml_string, &encoding_error_handler};
        cl7::text::codec::Decoder<cl7::u8char_t>::iterator end;

        for (; it != end; ++it)
        {
            auto codepoint = *it;

            if (codepoint.value < 0x20)
            {
                if ((codepoint.value == 0x09 || codepoint.value == 0x0a || codepoint.value == 0x0d) && unescape_type == UnescapeType::AttributeValue)
                    out << u8' ';
                else
                    out << static_cast<cl7::u8char_t>(codepoint.value);
                continue;
            }

            if (codepoint.value != U'&')
            {
                const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                out << cl7::u8string_view(utf8_buffer, size);
                continue;
            }

            size_t pos = it.pos();
            size_t count = 1;
            while (++it != end && it->value != ';')
                ++count;

            if (it == end || count == 1)
            {
                out << my_error_handler->handle_invalid_escape_sequence(xml_string.substr(pos, count));
                ++error_count;
                break;
            }

            auto ref = xml_string.substr(pos, ++count);
            assert(ref.size() >= 3);

            if (ref == u8"&amp;")
                out << u8'&';
            else if (ref == u8"&lt;")
                out << u8'<';
            else if (ref == u8"&gt;")
                out << u8'>';
            else if (ref == u8"&quot;")
                out << u8'"';
            else if (ref == u8"&apos;")
                out << u8'\'';
            else if (ref[1] == u8'#')
            {
                bool okay = true;

                if (ref[2] == u8'x')
                {
                    for (size_t i = 3; i < ref.size() - 1; ++i)
                        okay &= cl7::text::inspect::is_hex_digit(ref[i]);
                    codepoint.value = cl7::text::format::parse_hex(ref.substr(3, ref.size() - 4));
                }
                else
                {
                    for (size_t i = 3; i < ref.size() - 1; ++i)
                        okay &= cl7::text::inspect::is_digit(ref[i]);
                    codepoint.value = cl7::text::format::parse_int(ref.substr(2, ref.size() - 3));
                }

                if (okay)
                {
                    const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                    out << cl7::u8string_view(utf8_buffer, size);
                }
                else
                {
                    out << my_error_handler->handle_invalid_escape_sequence(ref);
                    ++error_count;
                }
            }
            else
            {
                out << my_error_handler->handle_invalid_escape_sequence(ref);
                ++error_count;
            }

        } // for each Unicode code point

        return error_count == 0;
    }



} // namespace dl7::xml::util
