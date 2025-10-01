#include "Utf16Codec.h"

#include "CoreLabs/text/codec/EncodeBuffer.h"



namespace cl7::text::codec {



    static size_t _determine_encode_length(codepoint codepoint)
    {
        assert(codepoint.is_valid_unicode());
        return 1 + static_cast<size_t>(codepoint.value > 0xffff);
    }

    static void _do_encode(codepoint codepoint, Utf16Codec::string_span_type output, size_t length)
    {
        assert(codepoint.is_valid_unicode());
        switch (length)
        {
        case 1:
            output[0] = static_cast<Utf16Codec::char_type>(codepoint.value);
            break;
        case 2:
        {
            const codepoint::value_type tmp = codepoint.value - 0x10000;
            assert((tmp >> 10) <= 0x3ff);
            output[0] = static_cast<Utf16Codec::char_type>(0xd800 | (tmp >> 10));
            output[1] = static_cast<Utf16Codec::char_type>(0xdc00 | (tmp & 0x3ff));
            break;
        }
        default:
            assert(false);
        }
    }

    static size_t _determine_decode_length(Utf16Codec::string_view_type input)
    {
        assert(!input.empty());
        assert(input[0] >= 0xd800 && input[0] <= 0xdbff);
        return 1 + static_cast<size_t>(input.size() >= 2 && (input[1] & 0xdc00) == 0xdc00); // check for trailing/low surrogate between 0xdc00 and 0xdfff
    }

    static codepoint _do_decode_2(Utf16Codec::string_view_type input)
    {
        assert(input.size() >= 2);
        assert(input[0] >= 0xd800 && input[0] <= 0xdbff);
        assert(input[1] >= 0xdc00 && input[1] <= 0xdfff);
        return {((static_cast<codepoint::value_type>(input[0] & 0x3ff) << 10) | static_cast<codepoint::value_type>(input[1] & 0x3ff)) + 0x10000};
    }



    size_t Utf16Codec::determine_encode_length(codepoint codepoint, codec::codepoint replacement)
    {
        if (codepoint.is_valid_unicode())
            return _determine_encode_length(codepoint);

        return _determine_encode_length(replacement);
    }

    size_t Utf16Codec::determine_decode_length(string_view_type input)
    {
        if (input.empty())
            return 0;

        const char_type first = input[0];

        if ((first & 0xfc00) == 0xd800) // leading/high surrogate (first >= 0xd800 && first <= 0xdbff)
            return _determine_decode_length(input);

        assert(first <= 0xd7ff || first >= 0xe000 || (first >= 0xdc00 && first <= 0xdfff)); // "regular" character or (unexpected) trailing/low surrogate
        return 1;
    }



    Utf16Codec::EncodeResult Utf16Codec::encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output, const ErrorHandler& error_handler)
    {
        auto codepoint_result = error_handler.check_adjust_unicode(error_status, codepoint);

        if (output.empty())
        {
            return {
                error_handler.process_exhausted_output_space(error_status, encoding, codepoint_result),
                {},
            };
        }

        assert(codepoint_result.codepoint.is_valid_unicode());
        size_t length = _determine_encode_length(codepoint_result.codepoint);
        assert(length >= 1 && length <= 2);

        if (length <= output.size()) // Output large enough for the entire sequence?
        {
            _do_encode(codepoint_result.codepoint, output, length);

            return {
                codepoint_result,
                output.subspan(0, length),
            };
        }

        // Provide an auxiliary buffer to
        // temporarily store the entire sequence.
        EncodeBuffer<char_type> buffer;

        _do_encode(codepoint_result.codepoint, buffer.string_span(), length);

        // Copy single leading/high surrogate code unit.
        assert(output.size() == 1);
        output[0] = buffer.data[0];

        return {
            error_handler.process_insufficient_output_space(error_status, encoding, codepoint_result, length, output.size()),
            output,
        };
    }



    Utf16Codec::DecodeResult Utf16Codec::decode_one(ErrorStatus& error_status, string_view_type input, const ErrorHandler& error_handler)
    {
        if (input.empty())
            return {};

        const char_type first = input[0];

        if ((first & 0xf800) != 0xd800) // "regular" character (first <= 0xd7ff || first >= 0xe000)
        {
            return {
                {static_cast<codepoint::value_type>(first)},
                input.substr(0, 1),
            };
        }

        if (/*first >= 0xd800 && */first <= 0xdbff) // leading/high surrogate
        {
            size_t length = _determine_decode_length(input);
            if (length != 2)
            {
                return error_handler.process_incomplete_sequence(error_status, input.substr(0, length));
            }

            return {
                _do_decode_2(input),
                input.substr(0, length),
            };
        }

        assert(first >= 0xdc00 && first <= 0xdfff); // (unexpected) trailing/low surrogate
        return error_handler.process_unpaired_surrogate(error_status, input.substr(0, 1));
    }



} // namespace cl7::text::codec
