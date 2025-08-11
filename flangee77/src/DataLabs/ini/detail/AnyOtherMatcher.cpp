#include "AnyOtherMatcher.h"



namespace dl7::ini::detail {



    size_t AnyOtherMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty())
            return 0;

        size_t i = 0;

        for (; i < source.size(); ++i)
        {
            const auto ch = source[i];

            switch (ch)
            {
            case u8'\t': return i;
            case u8'\n': return i;
            case u8'\r': return i;
            case u8' ': return i;
            //case u8'"': return i;
            case u8'#': return i;
            case u8'\'': return i;
            case u8'.': return i;
            case u8'/': return i;
            case u8':': return i;
            case u8';': return i;
            case u8'=': return i;
            case u8'[': return i;
            case u8'\\': return i;
            case u8']': return i;
            default:
                if (ch < 0x20)
                    return i;
            }
        } // for ...

        return i;
    }



} // namespace dl7::ini::detail
