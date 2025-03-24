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



    EncodeResult() = default;
    EncodeResult(strings::codepoint codepoint, string_span_type output_written) : CodepointResult(codepoint), output_written(output_written) {}
    EncodeResult(const CodepointResult& codepoint_result, string_span_type output_written) : CodepointResult(codepoint_result), output_written(output_written) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_ENCODERESULT_H
