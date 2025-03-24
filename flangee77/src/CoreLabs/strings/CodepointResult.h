#ifndef CL7_STRINGS_CODEPOINTRESULT_H
#define CL7_STRINGS_CODEPOINTRESULT_H

#include "./codepoint.h"



namespace cl7::strings {



struct CodepointResult
{
    /** The code point that was supposedly encoded/decoded. */
    codepoint codepoint = {0};



    CodepointResult() = default;
    CodepointResult(strings::codepoint codepoint) : codepoint(codepoint) {}
};



} // namespace cl7::strings

#endif // CL7_STRINGS_CODEPOINTRESULT_H
