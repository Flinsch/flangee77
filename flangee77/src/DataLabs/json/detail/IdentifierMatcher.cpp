#include "IdentifierMatcher.h"

#include <CoreLabs/strings.h>

#include <unordered_set>



namespace dl7::json::detail {



    size_t IdentifierMatcher::operator()(cl7::u8string_view source) const
    {
        // Empty identifiers are invalid.
        if (source.empty())
            return 0;

        // First character must be a letter (A-Z, a-z), underscore (_), or dollar sign ($).
        if (!cl7::strings::is_ascii_letter(source[0]) && source[0] != u8'_' && source[0] != u8'$')
            return 0;

        // Remaining characters can be letters, digits, underscores, or dollar signs.
        size_t i = 1;
        while (i < source.size() && (cl7::strings::is_ascii_letter(source[i]) || cl7::strings::is_digit(source[i]) || source[i] == u8'_' || source[i] == u8'$'))
            ++i;

        // JavaScript reserved words:
        // https://tc39.es/ecma262/#prod-ReservedWord
        const std::unordered_set<cl7::u8string_view> JS_RESERVED_WORDS = {
            u8"await", u8"break", u8"case", u8"catch", u8"class", u8"const", u8"continue", u8"debugger", u8"default",
            u8"delete", u8"do", u8"else", u8"enum", u8"export", u8"extends", u8"false", u8"finally", u8"for",
            u8"function", u8"if", u8"import", u8"in", u8"instanceof", u8"new", u8"null", u8"return", u8"super",
            u8"switch", u8"this", u8"throw", u8"true", u8"try", u8"typeof", u8"var", u8"void", u8"while", u8"with",
            u8"yield",
        };

        // Ensure the identifier is not a reserved JavaScript keyword.
        if (JS_RESERVED_WORDS.contains(source.substr(0, i)))
            return 0;

        return i;
    }



} // namespace dl7::json::detail
