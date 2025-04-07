#ifndef DL7_JSON_DETAIL_IDENTIFIERMATCHER_H
#define DL7_JSON_DETAIL_IDENTIFIERMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::json::detail {



/**
 * Matches an identifier (i.e., an unquoted object key). Not all conditions of
 * JavaScript's identifier naming rules are considered, as with JSON5, for example.
 * We allow identifiers that begin with a letter (A-Z or a-z), an underscore, or a
 * dollar sign; consist of letters, digits, underscores, and/or dollar signs; and do
 * not correspond to any reserved keyword (as per the ECMAScript specification).
 */
struct IdentifierMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_IDENTIFIERMATCHER_H
