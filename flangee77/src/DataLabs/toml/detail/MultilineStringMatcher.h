#ifndef DL7_TOML_DETAIL_MULTILINESTRINGMATCHER_H
#define DL7_TOML_DETAIL_MULTILINESTRINGMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::toml::detail {



/**
 * Matches a TOML multi-line string delimited by three quote characters (`"""..."""`
 * for basic strings or `'''...'''` for literal strings). Basic multi-line strings
 * support backslash escapes (including line-ending backslashes); literal ones do not.
 * A closing delimiter may be immediately preceded by up to two additional quote
 * characters that are part of the content.
 */
struct MultilineStringMatcher
{
    cl7::u8char_t quote_char = u8'"';
    bool allow_escapes = true;

    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_MULTILINESTRINGMATCHER_H
