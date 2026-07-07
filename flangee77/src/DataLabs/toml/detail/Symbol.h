#ifndef DL7_TOML_DETAIL_SYMBOL_H
#define DL7_TOML_DETAIL_SYMBOL_H

#include <DataLabs/syntax/SymbolId.h>



namespace dl7::toml::detail {



enum Symbol : syntax::SymbolId
{
    WHITESPACE = 1,
    NEWLINE,
    COMMENT_DELIMITER,
    DOT,
    EQUALS,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    COMMA,
    LEFT_BRACE,
    RIGHT_BRACE,

    QUOTED_STRING_LITERAL,
    MULTILINE_STRING_LITERAL,
    DATE_TIME_LITERAL,

    ANY_OTHER,
}; // enum Symbol



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_SYMBOL_H
