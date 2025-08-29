#ifndef CL7_STRINGS_CODEC_ASCIICODEC_H
#define CL7_STRINGS_CODEC_ASCIICODEC_H

#include "./codepoint.h"
#include "./Encoding.h"
#include "./EncodeResult.h"
#include "./DecodeResult.h"
#include "./ErrorHandler.h"

#include <CoreLabs/string.h>



namespace cl7::strings::codec {



class AsciiCodec
{

public:
    static constexpr Encoding encoding = Encoding::ASCII;
    static constexpr bool variable_length_encoding = false;
    static constexpr codepoint default_replacement = {codepoint::replacement_ascii};

    using char_type = cl7::achar_t;

    using EncodeResult = EncodeResult<char_type>;
    using DecodeResult = DecodeResult<char_type>;
    using string_span_type = EncodeResult::string_span_type;
    using string_view_type = DecodeResult::string_view_type;



    static size_t determine_encode_length(codepoint codepoint, codec::codepoint replacement = default_replacement) { return 1; }
    static size_t determine_decode_length(string_view_type input) { return input.empty() ? 0 : 1; }

    static EncodeResult encode_one(ErrorStatus& error_status, codepoint codepoint, string_span_type output, const ErrorHandler& error_handler);
    static DecodeResult decode_one(ErrorStatus& error_status, string_view_type input, const ErrorHandler& error_handler);

}; // class AsciiCodec



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_ASCIICODEC_H
