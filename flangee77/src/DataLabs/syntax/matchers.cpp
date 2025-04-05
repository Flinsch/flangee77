#include "matchers.h"

#include <CoreLabs/strings.h>



namespace dl7::syntax::matchers {



    static size_t _quoted_string_matcher(cl7::u8string_view source, cl7::u8char_t quote_char)
    {
        if (source.empty() || source[0] != quote_char)
            return 0;

        for (size_t i = 1; i < source.size(); ++i)
        {
            auto ch = source[i];
            if (ch == quote_char) return i + 1; // Return matched string length.
            if (ch == u8'\\') ++i; // Effectively skip next, escaped character.
        }

        return 0;
    }



    size_t SingleQuotedStringMatcher::operator()(cl7::u8string_view source) const
    {
        return _quoted_string_matcher(source, u8'\'');
    }

    size_t DoubleQuotedStringMatcher::operator()(cl7::u8string_view source) const
    {
        return _quoted_string_matcher(source, u8'"');
    }



    size_t IntegerLiteralMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty())
            return 0;

        size_t i = 0;
        if (sign_policy != SignPolicy::Unsigned && (source[i] == u8'-' || (sign_policy == SignPolicy::AllowPlusMinus && source[i] == u8'+')))
            ++i;

        const size_t start = i;
        while (i < source.size() && cl7::strings::is_digit(source[i]))
            ++i;

        if (i == start)
            return 0; // No digits at all.

        if (i - start == 1)
            return i; // A single digit is always okay (even a single 0).

        if (source[start] == u8'0')
            return 0; // Leading zeros are not okay.

        return i;
    }

    size_t FloatingPointLiteralMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty())
            return 0;

        size_t i = 0;
        if (sign_policy != SignPolicy::Unsigned && (source[i] == u8'-' || (sign_policy == SignPolicy::AllowPlusMinus && source[i] == u8'+')))
            ++i;

        const size_t start = i;
        while (i < source.size() && cl7::strings::is_digit(source[i]))
            ++i;

        if (i - start > 1 && source[start] == u8'0')
            return 0; // Leading zeros are not okay if further digits follow.

        bool has_dot = false;
        if (i < source.size() && source[i] == u8'.')
        {
            has_dot = true;

            ++i;
            while (i < source.size() && cl7::strings::is_digit(source[i]))
                ++i;
        }

        if (!has_dot && i == start)
            return 0; // Neither int nor float part.

        if (notation == FloatNotation::AllowScientific && i < source.size() && (source[i] | 0x20) == u8'e')
        {
            ++i;
            if (i < source.size() && (source[i] == u8'+' || source[i] == u8'-'))
                ++i;

            const size_t exp_start = i;
            while (i < source.size() && cl7::strings::is_digit(source[i]))
                ++i;

            if (i == exp_start)
                return 0; // No digits in exponent.
        }

        return i;
    }



    size_t IdentifierMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty())
            return 0;

        // First character must be a letter (A-Z, a-z) or underscore (_).
        if (!cl7::strings::is_ascii_letter(source[0]) && source[0] != u8'_')
            return 0;

        // Remaining characters can be letters, digits, or underscores.
        size_t i = 1;
        while (i < source.size() && (cl7::strings::is_ascii_letter(source[i]) || cl7::strings::is_digit(source[i]) || source[i] == u8'_'))
            ++i;

        return i;
    }



    size_t LineCommentMatcher::operator()(cl7::u8string_view source) const
    {
        if (!source.starts_with(prefix))
            return 0;

        size_t i = prefix.size();
        while (i < source.size() && source[i] != u8'\n' && source[i] != u8'\r')
            ++i;

        return i;
    }

    size_t BlockCommentMatcher::operator()(cl7::u8string_view source) const
    {
        if (!source.starts_with(open))
            return 0;

        size_t i = open.size();
        int depth = 1;
        int max_depth = depth;

        while (i + close.size() <= source.size())
        {
            if (allow_nesting && source.substr(i, open.size()) == open)
            {
                i += open.size();
                max_depth = ++depth;
            }
            else if (source.substr(i, close.size()) == close)
            {
                i += close.size();
                if (--depth == 0)
                    return i;
            }
            else
                ++i;
        }

        return max_depth > 1 ? 0 : source.size(); // Unclosed comment: not okay with nesting.
    }



} // namespace dl7::syntax::matchers
