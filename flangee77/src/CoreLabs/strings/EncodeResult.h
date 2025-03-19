#ifndef CL7_STRINGS_ENCODERESULT_H
#define CL7_STRINGS_ENCODERESULT_H
#include "./CodepointResult.h"

#include <CoreLabs/string.h>



namespace cl7::strings {



template <typename Tchar>
    requires(std::is_integral_v<Tchar>)
struct EncodeResult
    : CodepointResult
{
    using char_type = Tchar;
    using string_span_type = std::span<char_type>;



    /** The span of the output buffer that holds the encoded representation of the code point. */
    string_span_type output_written = {};



    /** Initializes an "empty" result with no errors. */
    EncodeResult() = default;

    /** Initializes a result with no errors. */
    EncodeResult(strings::codepoint codepoint, string_span_type output_written) : CodepointResult(codepoint), output_written(output_written) {}

    /** Initializes a result with the specified error code and an error counter of 1. */
    EncodeResult(EncodingError error, strings::codepoint codepoint, string_span_type output_written) : CodepointResult(error, codepoint), output_written(output_written) {}

    /** Initializes a forwarded result. */
    EncodeResult(const CodepointResult& codepoint_result, string_span_type output_written) : CodepointResult(codepoint_result), output_written(output_written) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_ENCODERESULT_H
