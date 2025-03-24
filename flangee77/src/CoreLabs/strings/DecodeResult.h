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



    DecodeResult() = default;
    DecodeResult(strings::codepoint codepoint, string_view_type input_read) : CodepointResult(codepoint), input_read(input_read) {}
    DecodeResult(const CodepointResult& codepoint_result, string_view_type input_read) : CodepointResult(codepoint_result), input_read(input_read) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_DECODERESULT_H
