#ifndef CL7_STRINGS_UTF16CODEC_H
#define CL7_STRINGS_UTF16CODEC_H

#include "./codepoint.h"
#include "./Encoding.h"
#include "./EncodeResult.h"
#include "./DecodeResult.h"
#include "./ErrorHandler.h"

#include <CoreLabs/string.h>



namespace cl7::strings {



class Utf16Codec
{

public:
    static constexpr Encoding encoding = Encoding::UTF16;

    using char_type = cl7::u16char_t;

    using EncodeResult = EncodeResult<char_type>;
    using DecodeResult = DecodeResult<char_type>;
    using string_span_type = EncodeResult::string_span_type;
    using string_view_type = DecodeResult::string_view_type;



    static size_t determine_encode_length(codepoint codepoint, strings::codepoint replacement = {codepoint::replacement_unicode});
    static size_t determine_decode_length(string_view_type input);

    static EncodeResult encode_one(codepoint codepoint, string_span_type output, const ErrorHandler& error_handler);
    static DecodeResult decode_one(string_view_type input, const ErrorHandler& error_handler);

}; // class Utf16Codec



} // namespace cl7::strings

#endif // CL7_STRINGS_UTF16CODEC_H
