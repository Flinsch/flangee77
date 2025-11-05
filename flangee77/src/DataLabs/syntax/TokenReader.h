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
     * Checks whether a valid token is present (i.e., with a positive symbol ID)
     * and EOF has not yet been reached.
     */
    bool is_valid() const;



    /**
     * Returns the ID of the symbol of the current token without advancing the
     * reader.
     */
    SymbolId peek_symbol_id() const;

    /**
     * Returns the current token's lexeme without advancing the reader.
     */
    cl7::u8string_view peek_lexeme() const;

    /**
     * Returns the first `n` characters of the current token's lexeme (can be
     * shorter).
     */
    cl7::u8string_view peek_prefix(size_t n) const;

    /**
     * Returns the first character of the current token's lexeme (or '\0' if
     * unavailable).
     */
    cl7::u8char_t peek_first_char() const;



    /**
     * Returns true if the current token's symbol ID matches the given one.
     */
    bool check_symbol_id(SymbolId symbol_id) const;

    /**
     * Returns true if the current token's lexeme matches the given one.
     */
    bool check_lexeme(cl7::u8string_view lexeme) const;

    /**
     * Returns true if the current token's lexeme starts with the given prefix.
     */
    bool check_prefix(cl7::u8string_view prefix) const;

    /**
     * Returns true if the first character of the current token's lexeme matches the
     * given one.
     */
    bool check_first_char(cl7::u8char_t first_char) const;



    /**
     * Advances to the next token if the current token's symbol ID matches the given
     * one. Returns true if advanced, false otherwise.
     */
    bool skip_symbol_id(SymbolId symbol_id);

    /**
     * Advances past all consecutive tokens whose symbol ID matches the given one.
     * Returns true if at least one token was skipped, false otherwise.
     */
    bool skip_while_symbol_id(SymbolId symbol_id);

    /**
     * Advances to the next token if the current token's lexeme matches the
     * given string. Returns true if advanced, false otherwise.
     */
    bool skip_lexeme(cl7::u8string_view lexeme);

    /**
     * Advances to the next token if the current token's lexeme starts with the
     * given prefix. Returns true if advanced, false otherwise.
     */
    bool skip_prefix(cl7::u8string_view prefix);

    /**
     * Advances to the next token if the first character of the current token's
     * lexeme matches the given one. Returns true if advanced, false otherwise.
     */
    bool skip_first_char(cl7::u8char_t first_char);



    /**
     * Skips all tokens until a token with the given symbol ID is found, such that
     * the current token has that symbol ID. If the symbol is not found, stops at
     * EOF.
     *
     * This function is primarily intended to assist with error recovery, allowing
     * the parser to resynchronize at a known token boundary, but may also be used
     * in other scenarios where such skipping is required, of course.
     *
     * Returns true if a token with the specified symbol ID was found, false
     * otherwise (i.e., EOF was reached).
     */
    bool skip_to_symbol_id(SymbolId symbol_id);

    /**
     * Skips all tokens until a token with the given symbol ID is found, and then
     * advances once more so that the current token is the one immediately following
     * it. If the symbol is not found, stops at EOF.
     *
     * This function is primarily intended to assist with error recovery, allowing
     * the parser to resynchronize at a known token boundary, but may also be used
     * in other scenarios where such skipping is required, of course.
     *
     * Returns true if a token with the specified symbol ID was found, false
     * otherwise (i.e., EOF was reached).
     */
    bool skip_past_symbol_id(SymbolId symbol_id);



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
     * Checks if the reader has reached the end of the token stream
     * (i.e., there are no more tokens or the "EOF" symbol has been reached).
     */
    virtual bool is_eof() const = 0;

}; // class TokenReader



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TOKENREADER_H
