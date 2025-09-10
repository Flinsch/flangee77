#include "Utf8Codec.h"

#include "CoreLabs/strings/codec/EncodeBuffer.h"



namespace cl7::strings::codec {



    static size_t _determine_encode_length(codepoint codepoint)
    {
        assert(codepoint.is_valid_unicode());
        return 1 + static_cast<size_t>(codepoint.value >= 0x80) + static_cast<size_t>(codepoint.value >= 0x800) + static_cast<size_t>(codepoint.value >= 0x10000);
    }

    static void _do_encode(codepoint codepoint, Utf8Codec::string_span_type output, size_t length)
    {
        assert(codepoint.is_valid_unicode());
        switch (length)
        {
        case 1:
            output[0] = static_cast<Utf8Codec::char_type>(codepoint.value);
            break;
        case 2:
            output[0] = static_cast<Utf8Codec::char_type>(0xc0 | ((codepoint.value >> 6) & 0x1f));
            output[1] = static_cast<Utf8Codec::char_type>(0x80 | (codepoint.value & 0x3f));
            break;
        case 3:
            output[0] = static_cast<Utf8Codec::char_type>(0xe0 | ((codepoint.value >> 12) & 0x0f));
            output[1] = static_cast<Utf8Codec::char_type>(0x80 | ((codepoint.value >> 6) & 0x3f));
            output[2] = static_cast<Utf8Codec::char_type>(0x80 | (codepoint.value & 0x3f));
            break;
        case 4:
            output[0] = static_cast<Utf8Codec::char_type>(0xf0 | ((codepoint.value >> 18) & 0x07));
            output[1] = static_cast<Utf8Codec::char_type>(0x80 | ((codepoint.value >> 12) & 0x3f));
            output[2] = static_cast<Utf8Codec::char_type>(0x80 | ((codepoint.value >> 6) & 0x3f));
            output[3] = static_cast<Utf8Codec::char_type>(0x80 | (codepoint.value & 0x3f));
            break;
        default:
            assert(false);
        }
    }

    static size_t _determine_decode_length(Utf8Codec::string_view_type input, size_t max)
    {
        assert(!input.empty());
        assert(input[0] >= 0xc0 && input[0] <= 0xfd);
        assert(max >= 2 && max <= 6);
        assert((input[0] >= 0xc0 && input[0] <= 0xdf) || max != 2);
        assert((input[0] >= 0xe0 && input[0] <= 0xef) || max != 3);
        assert((input[0] >= 0xf0 && input[0] <= 0xf7) || max != 4);
        assert((input[0] >= 0xf8 && input[0] <= 0xfb) || max != 5);
        assert((input[0] >= 0xfc && input[0] <= 0xfd) || max != 6);
        size_t length = 1;
        max = (std::min)(max, input.size());
        while (--max && (input[length] & 0xc0) == 0x80) // count continuation bytes between 0x80 and 0xbf
            ++length;
        return length;
    }

    static codepoint _do_decode_2(Utf8Codec::string_view_type input)
    {
        assert(input.size() >= 2);
        assert(input[0] >= 0xc2 && input[0] <= 0xdf);
        assert(input[1] >= 0x80 && input[1] <= 0xbf);
        return {((static_cast<codepoint::value_type>(input[0]) & 0x1f) << 6)
            | (static_cast<codepoint::value_type>(input[1]) & 0x3f)};
    }

    static codepoint _do_decode_3(Utf8Codec::string_view_type input)
    {
        assert(input.size() >= 3);
        assert(input[0] >= 0xe0 && input[0] <= 0xef);
        assert(input[1] >= 0x80 && input[1] <= 0xbf);
        assert(input[2] >= 0x80 && input[2] <= 0xbf);
        assert(input[0] != 0xe0 || input[1] >= 0xa0); // 0xe0 followed by less than 0xa0: overlong encoding
        return {((static_cast<codepoint::value_type>(input[0]) & 0x0f) << 12)
            | ((static_cast<codepoint::value_type>(input[1]) & 0x3f) << 6)
            | (static_cast<codepoint::value_type>(input[2]) & 0x3f)};
    }

    static codepoint _do_decode_4(Utf8Codec::string_view_type input)
    {
        assert(input.size() >= 4);
        assert(input[0] >= 0xf0 && input[0] <= 0xf4);
        assert(input[1] >= 0x80 && input[1] <= 0xbf);
        assert(input[2] >= 0x80 && input[2] <= 0xbf);
        assert(input[3] >= 0x80 && input[3] <= 0xbf);
        assert(input[0] != 0xf0 || input[1] >= 0x90); // 0xf0 followed by less than 0x90: overlong encoding
        assert(input[0] != 0xf4 || input[1] < 0x90); // 0xf4 followed by 0x90 or greater: value greater than U+10FFFF
        return {((static_cast<codepoint::value_type>(input[0]) & 0x07) << 18)
            | ((static_cast<codepoint::value_type>(input[1]) & 0x3f) << 12)
            | ((static_cast<codepoint::value_type>(input[2]) & 0x3f) << 6)
            | (static_cast<codepoint::value_type>(input[3]) & 0x3f)};
    }



    size_t Utf8Codec::determine_encode_length(codepoint codepoint, codec::codepoint replacement)
    {
        if (codepoint.is_valid_unicode())
            return _determine_encode_length(codepoint);

        return _determine_encode_length(replacement);
    }

    size_t Utf8Codec::determine_decode_length(string_view_type input)
    {
        if (input.empty())
            return 0;

        const char_type first = input[0];

        if (first <= 0xbf) // early out for ASCII (no sequence) or (unexpected) continuation byte
            return 1;

        if (/*first >= 0xc0 && */first <= 0xdf) // start of 2-byte sequence
            return _determine_decode_length(input, 2);
        if (/*first >= 0xe0 && */first <= 0xef) // start of 3-byte sequence
            return _determine_decode_length(input, 3);
        if (/*first >= 0xf0 && */first <= 0xf7) // start of 4-byte sequence
            return _determine_decode_length(input, 4);
        if (/*first >= 0xf8 && */first <= 0xfb) // start of (invalid) 5-byte sequence
            return _determine_decode_length(input, 5);
        if (/*first >= 0xfc && */first <= 0xfd) // start of (invalid) 6-byte sequence
            return _determine_decode_length(input, 6);

        assert(first >= 0xfe && first <= 0xff); // any (other) unused/invalid code unit
        return 1;
    }



    Utf8Codec::EncodeResult Utf8Codec::encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output, const ErrorHandler& error_handler)
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
        assert(length >= 1 && length <= 4);

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

        // Copy as many code units as possible.
        for (size_t i = 0; i < output.size(); ++i)
            output[i] = buffer.data[i];

        return {
            error_handler.process_insufficient_output_space(error_status, encoding, codepoint_result, length, output.size()),
            output,
        };
    }



    Utf8Codec::DecodeResult Utf8Codec::decode_one(ErrorStatus& error_status, string_view_type input, const ErrorHandler& error_handler)
    {
        if (input.empty())
            return {};

        const char_type first = input[0];

        if (first <= 0x7f) // early out for ASCII (no sequence)
        {
            return {
                {static_cast<codepoint::value_type>(first)},
                input.substr(0, 1),
            };
        }

        if ((first & 0xe0) == 0xc0) // start of 2-byte sequence (first >= 0xc0 && first <= 0xdf)
        {
            size_t length = _determine_decode_length(input, 2);
            if (length != 2)
            {
                return error_handler.process_incomplete_sequence(error_status, input.substr(0, length), 2);
            }

            if (first < 0xc2) // overlong encoding (0 through 0x7f)
            {
                return error_handler.process_overlong_encoding(error_status, input.substr(0, length));
            }

            return {
                _do_decode_2(input),
                input.substr(0, length),
            };
        }

        if ((first & 0xf0) == 0xe0) // start of 3-byte sequence (first >= 0xe0 && first <= 0xef)
        {
            size_t length = _determine_decode_length(input, 3);
            if (length != 3)
            {
                return error_handler.process_incomplete_sequence(error_status, input.substr(0, length), 3);
            }

            if (first == 0xe0 && input[1] < 0xa0) // overlong encoding (0 through 0x7ff)
            {
                return error_handler.process_overlong_encoding(error_status, input.substr(0, length));
            }

            if (first == 0xed && input[1] >= 0xa0) // surrogate range 0xd800 through 0xdfff
            {
                return error_handler.process_invalid_sequence(error_status, input.substr(0, length));
            }

            return {
                _do_decode_3(input),
                input.substr(0, length),
            };
        }

        if (first >= 0xf0 && first <= 0xf4) // start of 4-byte sequence
        {
            size_t length = _determine_decode_length(input, 4);
            if (length != 4)
            {
                return error_handler.process_incomplete_sequence(error_status, input.substr(0, length), 4);
            }

            if (first == 0xf0 && input[1] < 0x90) // overlong encoding (0 through 0xffff)
            {
                return error_handler.process_overlong_encoding(error_status, input.substr(0, length));
            }

            if (first == 0xf4 && input[1] >= 0x90) // code range above 0x10ffff
            {
                return error_handler.process_invalid_sequence(error_status, input.substr(0, length));
            }

            return {
                _do_decode_4(input),
                input.substr(0, length),
            };
        }

        if (/*first >= 0x80 && */first <= 0xbf) // unexpected continuation byte of unknown sequence
        {
            return error_handler.process_invalid_code_unit(error_status, input.substr(0, 1));
        }

        if (/*first >= 0xf5 && */first <= 0xf7) // start of invalid 4-byte sequence (representing values of 0x140000 and above)
        {
            size_t length = _determine_decode_length(input, 4);
            return error_handler.process_invalid_sequence(error_status, input.substr(0, length));
        }

        if (/*first >= 0xf8 && */first <= 0xfb) // start of invalid 5-byte sequence
        {
            size_t length = _determine_decode_length(input, 5);
            return error_handler.process_invalid_sequence(error_status, input.substr(0, length));
        }

        if (/*first >= 0xfc && */first <= 0xfd) // start of invalid 6-byte sequence
        {
            size_t length = _determine_decode_length(input, 6);
            return error_handler.process_invalid_sequence(error_status, input.substr(0, length));
        }

        assert(first >= 0xfe && first <= 0xff);
        return error_handler.process_invalid_code_unit(error_status, input.substr(0, 1));
    }



} // namespace cl7::strings::codec
