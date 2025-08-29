#ifndef CL7_STRINGS_CODEC_CODEPOINTRESULT_H
#define CL7_STRINGS_CODEC_CODEPOINTRESULT_H

#include "./codepoint.h"



namespace cl7::strings::codec {



struct CodepointResult
{
    /** The code point that was supposedly encoded/decoded. */
    codepoint codepoint = {0};



    CodepointResult() = default;
    CodepointResult(codec::codepoint codepoint) : codepoint(codepoint) {}
};



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_CODEPOINTRESULT_H
