#include "AsciiCodec.h"



namespace cl7::strings {



    AsciiCodec::EncodeResult AsciiCodec::encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output, const ErrorHandler& error_handler)
    {
        auto codepoint_result = error_handler.check_adjust_ascii(error_status, codepoint);

        if (output.empty())
        {
            return {
                error_handler.on_exhausted_output_space(error_status, encoding, codepoint_result),
                {},
            };
        }

        assert(codepoint_result.codepoint.is_valid_ascii());
        output[0] = static_cast<char_type>(codepoint_result.codepoint.value & 0x7f);

        return {
            codepoint_result,
            output.subspan(0, 1),
        };
    }



    AsciiCodec::DecodeResult AsciiCodec::decode_one(ErrorStatus& error_status, string_view_type input, const ErrorHandler& error_handler)
    {
        if (input.empty())
            return {};

        if (input[0] & 0x80)
        {
            return error_handler.on_invalid_code_unit(error_status, input.substr(0, 1));
        }

        return {
            {static_cast<codepoint::value_type>(static_cast<uint8_t>(input[0]))},
            input.substr(0, 1),
        };
    }



} // namespace cl7::strings
