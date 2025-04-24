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
     * Returns the ID of the symbol of the current token without advancing the
     * reader.
     */
    SymbolID peek_symbol_id() const;

    /**
     * Returns the first character of the current token's lexeme (or '\0' if
     * unavailable).
     */
    cl7::u8char_t peek_first_char() const;

    /**
     * Returns the first `n` characters of the current token's lexeme (can be
     * shorter).
     */
    cl7::u8string_view peek_prefix(size_t n) const;



    /**
     * Returns true if the current token's symbol ID matches the given one.
     */
    bool check_symbol_id(SymbolID symbol_id) const;

    /**
     * Returns true if the first character of the current token's lexeme matches the
     * given one.
     */
    bool check_first_char(cl7::u8char_t first_char) const;

    /**
     * Returns true if the current token's lexeme starts with the given prefix.
     */
    bool check_prefix(cl7::u8string_view prefix) const;



    /**
     * Advances to the next token if the current token's symbol ID matches the given
     * one. Returns true if advanced, false otherwise.
     */
    bool skip_symbol_id(SymbolID symbol_id);

    /**
     * Advances to the next token if the first character of the current token's
     * lexeme matches the given one. Returns true if advanced, false otherwise.
     */
    bool skip_first_char(cl7::u8char_t first_char);

    /**
     * Advances to the next token if the current token's lexeme starts with the
     * given prefix. Returns true if advanced, false otherwise.
     */
    bool skip_prefix(cl7::u8string_view prefix);



    /**
     * Advances to the next token, but returns the token that was current before
     * advancing.
     */
    Token consume_token();



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
