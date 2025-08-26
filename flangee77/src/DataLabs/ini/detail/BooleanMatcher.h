#ifndef DL7_INI_DETAIL_BOOLEANMATCHER_H
#define DL7_INI_DETAIL_BOOLEANMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::ini::detail {



struct BooleanMatcher
{
    enum struct Mode
    {
        TruthyOnly,
        FalsyOnly,
        Both,
    };

    Mode mode = Mode::Both;

    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::ini::detail

#endif // DL7_INI_DETAIL_BOOLEANMATCHER_H
