#ifndef CL7_TEXT_CODEC_DECODERESULT_H
#define CL7_TEXT_CODEC_DECODERESULT_H
#include "./CodepointResult.h"

#include <CoreLabs/string.h>



namespace cl7::text::codec {



template <any_char Tchar>
struct DecodeResult
    : CodepointResult
{
    using char_type = Tchar;
    using string_view_type = std::basic_string_view<char_type>;



    /** The portion of the input string that was decoded. */
    string_view_type input_read = {};



    DecodeResult() = default;
    DecodeResult(codec::codepoint codepoint, string_view_type input_read) : CodepointResult(codepoint), input_read(input_read) {}
    DecodeResult(const CodepointResult& codepoint_result, string_view_type input_read) : CodepointResult(codepoint_result), input_read(input_read) {}
};



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_DECODERESULT_H
