#ifndef CL7_STRINGS_DECODERESULT_H
#define CL7_STRINGS_DECODERESULT_H
#include "./CodepointResult.h"

#include <CoreLabs/string.h>



namespace cl7::strings {



template <typename Tchar>
    requires(std::is_integral_v<Tchar>)
struct DecodeResult
    : CodepointResult
{
    using char_type = Tchar;
    using string_view_type = std::basic_string_view<char_type>;



    /** The portion of the input string that was decoded. */
    string_view_type input_read = {};



    /** Initializes an "empty" result with no errors. */
    DecodeResult() = default;

    /** Initializes a result with no errors. */
    DecodeResult(strings::codepoint codepoint, string_view_type input_read) : CodepointResult(codepoint), input_read(input_read) {}

    /** Initializes a result with the specified error code and an error counter of 1. */
    DecodeResult(EncodingError error, strings::codepoint codepoint, string_view_type input_read) : CodepointResult(error, codepoint), input_read(input_read) {}

    /** Initializes a forwarded result. */
    DecodeResult(const CodepointResult& codepoint_result, string_view_type input_read) : CodepointResult(codepoint_result), input_read(input_read) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_DECODERESULT_H
