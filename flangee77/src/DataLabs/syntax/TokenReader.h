#ifndef DL7_SYNTAX_TOKENREADER_H
#define DL7_SYNTAX_TOKENREADER_H

#include "./Token.h"



namespace dl7::syntax {



/**
 * An abstract class/interface for reading tokens from a source. Provides methods
 * for retrieving tokens sequentially while allowing for optional lookahead.
 * Concrete implementations may either fetch tokens from a pre-generated buffer or
 * generate them dynamically via a lexer.
 */
class TokenReader
{

public:
    virtual ~TokenReader() = default;



    /**
     * Returns the ID of the symbol of the current token without advancing the reader.
     */
    SymbolID peek_symbol_id() const { return peek_token().symbol_id; }

    /**
     * Returns the current token without advancing the reader.
     */
    virtual const Token& peek_token() const = 0;

    /**
     * Advances the reader and returns the next token.
     */
    virtual const Token& next_token() = 0;

    /**
     * Checks if the reader has reached the end of the token stream.
     */
    virtual bool is_eof() const = 0;

}; // class TokenReader



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TOKENREADER_H
