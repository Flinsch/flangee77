#ifndef CL7_TEXT_CODEC_CODEPOINTRESULT_H
#define CL7_TEXT_CODEC_CODEPOINTRESULT_H

#include "./codepoint.h"



namespace cl7::text::codec {



struct CodepointResult
{
    /** The code point that was supposedly encoded/decoded. */
    codepoint codepoint = {0};



    CodepointResult() = default;
    CodepointResult(codec::codepoint codepoint) : codepoint(codepoint) {}
};



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_CODEPOINTRESULT_H
