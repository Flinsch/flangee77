#ifndef DL7_YAML_DETAIL_SINGLEQUOTEDSTRINGMATCHER_H
#define DL7_YAML_DETAIL_SINGLEQUOTEDSTRINGMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::yaml::detail {



/**
 * Matches a single-quoted scalar. Unlike the generic single-quoted string matcher,
 * this one implements YAML's rule that the only escape available inside such a
 * scalar is a doubled quote (`''`), backslashes being ordinary characters.
 *
 * Scalars spanning multiple lines are not matched (they are not supported).
 */
struct SingleQuotedStringMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_SINGLEQUOTEDSTRINGMATCHER_H
