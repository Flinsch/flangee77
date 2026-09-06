#ifndef DL7_YAML_DETAIL_ANYOTHERMATCHER_H
#define DL7_YAML_DETAIL_ANYOTHERMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::yaml::detail {



/**
 * Matches a run of characters that carry no structural meaning on their own, i.e.,
 * the raw material plain scalars are made of. It deliberately stops at every
 * character that *may* be structural somewhere (`:`, `#`, `,`, brackets, braces,
 * quotes), even though most of them are perfectly legal inside a plain scalar: the
 * lexer cannot tell, so the builder decides and reassembles the scalar from the
 * resulting token run.
 *
 * Characters that are structural only at the very beginning of a node (`-`, `?`,
 * `|`, `>`, `&`, `*`, `!`) are *not* stopped at: there they end up as one-character
 * matches that lose the longest-match rule to the respective literal anyway.
 */
struct AnyOtherMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_ANYOTHERMATCHER_H
