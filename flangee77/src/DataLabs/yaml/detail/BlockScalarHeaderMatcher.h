#ifndef DL7_YAML_DETAIL_BLOCKSCALARHEADERMATCHER_H
#define DL7_YAML_DETAIL_BLOCKSCALARHEADERMATCHER_H

#include <CoreLabs/string.h>



namespace dl7::yaml::detail {



/**
 * Matches the header of a block scalar: `|` (literal) or `>` (folded), followed by
 * up to one indentation indicator (a digit) and up to one chomping indicator (`+`
 * or `-`), in either order.
 *
 * A plain literal would not do here: against `|-`, the longest-match rule would let
 * the plain scalar matcher win. This one has to be registered ahead of the latter,
 * so that it wins the tie for a bare `|` or `>`.
 */
struct BlockScalarHeaderMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_BLOCKSCALARHEADERMATCHER_H
