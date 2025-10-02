#ifndef CL7_TEXT_CODEC_ENCODERESULT_H
#define CL7_TEXT_CODEC_ENCODERESULT_H
#include "./CodepointResult.h"

#include <CoreLabs/string.h>



namespace cl7::text::codec {



template <std::integral Tchar>
struct EncodeResult
    : CodepointResult
{
    using char_type = Tchar;
    using string_span_type = std::span<char_type>;



    /** The span of the output buffer that holds the encoded representation of the code point. */
    string_span_type output_written = {};



    EncodeResult() = default;
    EncodeResult(codec::codepoint codepoint, string_span_type output_written) : CodepointResult(codepoint), output_written(output_written) {}
    EncodeResult(const CodepointResult& codepoint_result, string_span_type output_written) : CodepointResult(codepoint_result), output_written(output_written) {}
};



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_ENCODERESULT_H
