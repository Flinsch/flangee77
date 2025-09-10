#ifndef DL7_INI_DETAIL_SYMBOL_H
#define DL7_INI_DETAIL_SYMBOL_H

#include <DataLabs/syntax/SymbolID.h>



namespace dl7::ini::detail {



enum Symbol : syntax::SymbolID
{
    WHITESPACE = 1,
    NEWLINE,
    COMMENT_DELIMITER,
    SECTION_NAME_DELIMITER,
    KEY_VALUE_DELIMITER,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    ESCAPED_CHARACTER,
    ESCAPED_NEWLINE,

    QUOTED_STRING_LITERAL,

    ANY_OTHER,
}; // enum Symbol



} // namespace dl7::ini::detail

#endif // DL7_INI_DETAIL_SYMBOL_H
