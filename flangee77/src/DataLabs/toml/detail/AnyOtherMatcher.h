#ifndef DL7_TOML_DETAIL_ANYOTHERMATCHER_H
#define DL7_TOML_DETAIL_ANYOTHERMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::toml::detail {



/**
 * Matches a maximal run of "bare" characters that make up an unquoted key or a
 * bare value (integers, floats, booleans, dates/times, etc.). Stops at whitespace,
 * line breaks, and any TOML structural character (which are lexed as their own
 * tokens). Note that the dot (`.`) is intentionally excluded so that it remains a
 * separate token for dotted keys.
 */
struct AnyOtherMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_ANYOTHERMATCHER_H
