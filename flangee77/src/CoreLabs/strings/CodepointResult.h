#ifndef CL7_STRINGS_CODEPOINTRESULT_H
#define CL7_STRINGS_CODEPOINTRESULT_H
#include "./BaseResult.h"

#include "./codepoint.h"



namespace cl7::strings {



struct CodepointResult
    : BaseResult
{
    /** The code point that was supposedly encoded/decoded. */
    codepoint codepoint = {0};



    /** Initializes a result with no errors. */
    CodepointResult() = default;

    /** Initializes a result with no errors. */
    CodepointResult(strings::codepoint codepoint) : codepoint(codepoint) {}

    /** Initializes a result with the specified error code and an error counter of 1. */
    CodepointResult(EncodingError error, strings::codepoint codepoint) : BaseResult(error), codepoint(codepoint) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_CODEPOINTRESULT_H
