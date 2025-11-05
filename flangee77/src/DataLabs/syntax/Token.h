#ifndef DL7_SYNTAX_TOKEN_H
#define DL7_SYNTAX_TOKEN_H

#include "./TerminalSymbol.h"

#include <CoreLabs/string.h>



namespace dl7::syntax {



/**
 * Represents a lexical token produced by a lexer.
 */
struct Token
{

    /** The terminal symbol ID of the token. */
    SymbolId symbol_id;

    /** The piece of the source text that matches the terminal symbol definition. */
    cl7::u8string_view lexeme;

    /** The offset position of the token's lexeme in the source text (in terms of code units). */
    size_t source_offset;

}; // struct Token



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TOKEN_H
