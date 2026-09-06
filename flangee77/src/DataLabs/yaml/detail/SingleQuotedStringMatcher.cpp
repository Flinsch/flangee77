#include "SingleQuotedStringMatcher.h"



namespace dl7::yaml::detail {



    size_t SingleQuotedStringMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty() || source[0] != u8'\'')
            return 0;

        for (size_t i = 1; i < source.size(); ++i)
        {
            const auto ch = source[i];

            if (ch == u8'\n' || ch == u8'\r')
                return 0; // Unterminated (as far as we are concerned).

            if (ch != u8'\'')
                continue;

            if (i + 1 < source.size() && source[i + 1] == u8'\'')
            {
                ++i; // Skip the second quote of an escaped pair.
                continue;
            }

            return i + 1; // Return matched string length.
        }

        return 0;
    }



} // namespace dl7::yaml::detail
