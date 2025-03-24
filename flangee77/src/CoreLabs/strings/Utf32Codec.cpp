#include "Utf32Codec.h"



namespace cl7::strings {



    Utf32Codec::EncodeResult Utf32Codec::encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output, const ErrorHandler& error_handler)
    {
        auto codepoint_result = error_handler.check_adjust_unicode(error_status, codepoint);

        if (output.empty())
        {
            return {
                error_handler.on_exhausted_output_space(error_status, encoding, codepoint_result),
                {},
            };
        }

        assert(codepoint_result.codepoint.is_valid_unicode());
        output[0] = static_cast<char_type>(codepoint_result.codepoint.value);

        return {
            codepoint_result,
            output.subspan(0, 1),
        };
    }



    Utf32Codec::DecodeResult Utf32Codec::decode_one(ErrorStatus& error_status, string_view_type input, const ErrorHandler& error_handler)
    {
        if (input.empty())
            return {};

        codepoint codepoint = {static_cast<codepoint::value_type>(input[0])};

        if (!codepoint.is_valid_unicode())
        {
            return error_handler.on_invalid_code_unit(error_status, input.substr(0, 1));
        }

        return {
            codepoint,
            input.substr(0, 1),
        };
    }



} // namespace cl7::strings
