#ifndef CL7_STRINGS_ASCIICODEC_H
#define CL7_STRINGS_ASCIICODEC_H

#include "./codepoint.h"
#include "./Encoding.h"
#include "./EncodeResult.h"
#include "./DecodeResult.h"
#include "./ErrorHandler.h"

#include <CoreLabs/string.h>



namespace cl7::strings {



class AsciiCodec
{

public:
    static constexpr Encoding encoding = Encoding::ASCII;

    using char_type = cl7::achar_t;

    using EncodeResult = EncodeResult<char_type>;
    using DecodeResult = DecodeResult<char_type>;
    using string_span_type = EncodeResult::string_span_type;
    using string_view_type = DecodeResult::string_view_type;



    static EncodeResult encode_one(codepoint codepoint, string_span_type output, const ErrorHandler& error_handler);
    static DecodeResult decode_one(string_view_type input, const ErrorHandler& error_handler);

}; // class AsciiCodec



} // namespace cl7::strings

#endif // CL7_STRINGS_ASCIICODEC_H
