#ifndef CL7_STRINGS_CODEC_ENCODING_H
#define CL7_STRINGS_CODEC_ENCODING_H

#include <CoreLabs/root.h>



namespace cl7::strings::codec {



    // NOLINTBEGIN(readability-enum-initial-value)
    enum struct Encoding
    {
        Unknown,

        ASCII,
        UTF8,
        UTF16,
        UTF32,

#if WCHAR_MAX == UINT16_MAX
        UTFx = UTF16,
#elif WCHAR_MAX == UINT32_MAX
        UTFx = UTF32,
#endif

        Default = UTF8,
    };
    // NOLINTEND(readability-enum-initial-value)



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_ENCODING_H
