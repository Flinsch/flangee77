#include "Escaper.h"

#include <CoreLabs/text/codec/Encoder.h>
#include <CoreLabs/text/codec/Decoder.h>
#include <CoreLabs/text/format.h>



namespace dl7::xml::util {



    /**
     * Escapes a raw string for inclusion in XML content or attribute values and
     * returns the result as a new string.
     */
    cl7::u8string Escaper::escape_string(cl7::u8string_view raw_string, EscapeType escape_type, const Options& options) const
    {
        cl7::u8osstream out;
        escape_string(out, raw_string, escape_type, options);
        return out.str();
    }

    /**
     * Escapes a raw string for inclusion in XML content or attribute values and
     * writes the result to the provided output stream.
     */
    void Escaper::escape_string(cl7::u8osstream& out, cl7::u8string_view raw_string, EscapeType escape_type, const Options& options) const
    {
        cl7::text::codec::DefaultErrorHandler encoding_error_handler{_log_context};

        cl7::text::codec::Encoder<cl7::u8char_t> utf8_encoder{&encoding_error_handler};
        cl7::u8char_t utf8_buffer[4];

        cl7::text::codec::Decoder<cl7::u8char_t>::iterator it{raw_string, &encoding_error_handler};
        cl7::text::codec::Decoder<cl7::u8char_t>::iterator end;

        for (; it != end; ++it)
        {
            const auto codepoint = *it;

            if (codepoint.value < 0x20)
            {
                if (codepoint.value == 0x09 || codepoint.value == 0x0a || codepoint.value == 0x0d)
                {
                    if (escape_type == EscapeType::ElementContent)
                        out << static_cast<cl7::u8char_t>(codepoint.value);
                    else
                        out << u8' ';
                }
                else
                    out << u8"&#x" << cl7::text::format::to_hex_lc(codepoint.value, 2) << u8';';
                continue;
            }

            if (codepoint.value == U'&')
                out << u8"&amp;";
            else if (codepoint.value == U'<')
                out << u8"&lt;";
            else if (codepoint.value == U'>' && options.force_escape)
                out << u8"&gt;";
            else if (codepoint.value == U'"' && (escape_type == EscapeType::DoubleQuotedAttributeValue || options.force_escape))
                out << u8"&quot;";
            else if (codepoint.value == U'\'' && (escape_type == EscapeType::SingleQuotedAttributeValue || options.force_escape))
                out << u8"&apos;";
            else if (codepoint.value > 0x7f && options.escape_unicode)
            {
                out << u8"&#x" << cl7::text::format::to_hex_lc(codepoint.value, 4) << u8';';
            }
            else
            {
                const auto size = utf8_encoder.encode_one_into(codepoint, cl7::u8string_span(utf8_buffer, 4));
                out << cl7::u8string_view(utf8_buffer, size);
            }

        } // for each Unicode code point
    }



} // namespace dl7::xml::util
