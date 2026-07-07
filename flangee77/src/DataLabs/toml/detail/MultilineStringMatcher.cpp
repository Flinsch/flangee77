#include "MultilineStringMatcher.h"



namespace dl7::toml::detail {



    size_t MultilineStringMatcher::operator()(cl7::u8string_view source) const
    {
        // The string must open with three quote characters.
        if (source.size() < 3 || source[0] != quote_char || source[1] != quote_char || source[2] != quote_char)
            return 0;

        size_t i = 3;

        while (i < source.size())
        {
            const auto ch = source[i];

            if (allow_escapes && ch == u8'\\')
            {
                // Skip the backslash and the escaped character (whatever it is; this is
                // only about finding the closing delimiter, not interpreting escapes).
                i += 2;
                continue;
            }

            if (ch == quote_char)
            {
                size_t j = i;
                while (j < source.size() && source[j] == quote_char)
                    ++j;
                const size_t run = j - i;

                // A run of three or more quotes closes the string; the closing delimiter
                // is the last three quotes of the run (any extra ones are content).
                if (run >= 3)
                    return j;

                // A run of one or two quotes is just content.
                i = j;
                continue;
            }

            ++i;
        }

        // No closing delimiter found.
        return 0;
    }



} // namespace dl7::toml::detail
