#ifndef DL7_JSON_DETAIL_SYMBOL_H
#define DL7_JSON_DETAIL_SYMBOL_H

#include <DataLabs/syntax/SymbolID.h>



namespace dl7::json::detail {



enum Symbol : syntax::SymbolID
{
    NULL_CONSTANT = 1,
    TRUE_CONSTANT,
    FALSE_CONSTANT,

    INTEGER_LITERAL,
    DECIMAL_LITERAL,
    STRING_LITERAL,

    IDENTIFIER,

    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,

    COMMA,
    COLON,

    LINE_COMMENT,
    BLOCK_COMMENT,
}; // enum Symbol



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_SYMBOL_H
