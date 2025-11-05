#include "TokenReader.h"



namespace dl7::syntax {



    /**
     * Checks whether a valid token is present (i.e., with a positive symbol ID)
     * and EOF has not yet been reached.
     */
    bool TokenReader::is_valid() const
    {
        return !is_eof() && peek_token().symbol_id > 0;
    }



    /**
     * Returns the ID of the symbol of the current token without advancing the
     * reader.
     */
    SymbolId TokenReader::peek_symbol_id() const
    {
        return peek_token().symbol_id;
    }

    /**
     * Returns the current token's lexeme without advancing the reader.
     */
    cl7::u8string_view TokenReader::peek_lexeme() const
    {
        return peek_token().lexeme;
    }

    /**
     * Returns the first `n` characters of the current token's lexeme (can be
     * shorter).
     */
    cl7::u8string_view TokenReader::peek_prefix(size_t n) const
    {
        return peek_token().lexeme.substr(0, n);
    }

    /**
     * Returns the first character of the current token's lexeme (or '\0' if
     * unavailable).
     */
    cl7::u8char_t TokenReader::peek_first_char() const
    {
        assert(!peek_token().lexeme.empty());
        if (peek_token().lexeme.empty())
            return u8'\0';
        return peek_token().lexeme[0];
    }



    /**
     * Returns true if the current token's symbol ID matches the given one.
     */
    bool TokenReader::check_symbol_id(SymbolId symbol_id) const
    {
        return peek_symbol_id() == symbol_id;
    }

    /**
     * Returns true if the current token's lexeme matches the given one.
     */
    bool TokenReader::check_lexeme(cl7::u8string_view lexeme) const
    {
        return peek_token().lexeme == lexeme;
    }

    /**
     * Returns true if the current token's lexeme starts with the given prefix.
     */
    bool TokenReader::check_prefix(cl7::u8string_view prefix) const
    {
        return peek_token().lexeme.starts_with(prefix);
    }

    /**
     * Returns true if the first character of the current token's lexeme matches the
     * given one.
     */
    bool TokenReader::check_first_char(cl7::u8char_t first_char) const
    {
        return peek_first_char() == first_char;
    }



    /**
     * Advances to the next token if the current token's symbol ID matches the given
     * one. Returns true if advanced, false otherwise.
     */
    bool TokenReader::skip_symbol_id(SymbolId symbol_id)
    {
        if (check_symbol_id(symbol_id))
        {
            next_token();
            return true;
        }
        return false;
    }

    /**
     * Advances past all consecutive tokens whose symbol ID matches the given one.
     * Returns true if at least one token was skipped, false otherwise.
     */
    bool TokenReader::skip_while_symbol_id(SymbolId symbol_id)
    {
        bool skipped = false;
        while (check_symbol_id(symbol_id))
        {
            next_token();
            skipped = true;
        }
        return skipped;
    }

    /**
     * Advances to the next token if the current token's lexeme matches the
     * given string. Returns true if advanced, false otherwise.
     */
    bool TokenReader::skip_lexeme(cl7::u8string_view lexeme)
    {
        if (check_lexeme(lexeme))
        {
            next_token();
            return true;
        }
        return false;
    }

    /**
     * Advances to the next token if the current token's lexeme starts with the
     * given prefix. Returns true if advanced, false otherwise.
     */
    bool TokenReader::skip_prefix(cl7::u8string_view prefix)
    {
        if (check_prefix(prefix))
        {
            next_token();
            return true;
        }
        return false;
    }

    /**
     * Advances to the next token if the first character of the current token's
     * lexeme matches the given one. Returns true if advanced, false otherwise.
     */
    bool TokenReader::skip_first_char(cl7::u8char_t first_char)
    {
        if (check_first_char(first_char))
        {
            next_token();
            return true;
        }
        return false;
    }



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
    bool TokenReader::skip_to_symbol_id(SymbolId symbol_id)
    {
        while (!is_eof() && !check_symbol_id(symbol_id))
            next_token();
        return !is_eof() && check_symbol_id(symbol_id);
    }

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
    bool TokenReader::skip_past_symbol_id(SymbolId symbol_id)
    {
        if (!skip_to_symbol_id(symbol_id))
            return false;
        next_token();
        return !is_eof();
    }



    /**
     * Advances to the next token, but returns the token that was current before
     * advancing.
     */
    Token TokenReader::consume_token()
    {
        auto token = peek_token();
        next_token();
        return token;
    }



} // namespace dl7::syntax
