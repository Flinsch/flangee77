#ifndef DL7_SYNTAX_MATCHERS_H
#define DL7_SYNTAX_MATCHERS_H

#include <CoreLabs/string.h>



namespace dl7::syntax::matchers {



/**
 * Matches a single-quoted string with support for backslash escapes.
 */
struct SingleQuotedStringMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};

/**
 * Matches a double-quoted string with support for backslash escapes.
 */
struct DoubleQuotedStringMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



enum struct SignPolicy { Unsigned, AllowMinus, AllowPlusMinus };

/**
 * Matches an integer literal (e.g., 42, 0, -7, +1).
 */
struct IntegerLiteralMatcher
{
    SignPolicy sign_policy = SignPolicy::AllowPlusMinus;

    size_t operator()(cl7::u8string_view source) const;
};

enum struct FloatNotation { DecimalOnly, AllowScientific };

/**
 * Matches a floating-point literal (e.g., 3.14, -0.5, 2e10).
 */
struct FloatingPointLiteralMatcher
{
    SignPolicy sign_policy = SignPolicy::AllowPlusMinus;
    FloatNotation notation = FloatNotation::AllowScientific;

    size_t operator()(cl7::u8string_view source) const;
};



/**
 * Matches an identifier (e.g., variable name or keyword), starting with a letter or
 * underscore, followed by letters, digits, or underscores.
 */
struct IdentifierMatcher
{
    size_t operator()(cl7::u8string_view source) const;
};



/**
 * Matches a line comment with a specified delimiter (default: //).
 */
struct LineCommentMatcher
{
    cl7::u8string_view prefix = u8"//";

    size_t operator()(cl7::u8string_view source) const;
};

/**
 * Matches a block comment with configurable start and end delimiters (default: /* ... *\/),
 * with optional support for nesting.
 */
struct BlockCommentMatcher
{
    cl7::u8string_view open = u8"/*";
    cl7::u8string_view close = u8"*/";
    bool allow_nesting = false;

    size_t operator()(cl7::u8string_view source) const;
};



} // namespace dl7::syntax::matchers

#endif // DL7_SYNTAX_MATCHERS_H
