#include "TokenReader.h"



namespace dl7::syntax {



    /**
     * Returns the ID of the symbol of the current token without advancing the
     * reader.
     */
    SymbolID TokenReader::peek_symbol_id() const
    {
        return peek_token().symbol_id;
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
     * Returns the first `n` characters of the current token's lexeme (can be
     * shorter).
     */
    cl7::u8string_view TokenReader::peek_prefix(size_t n) const
    {
        return peek_token().lexeme.substr(0, n);
    }



    /**
     * Returns true if the current token's symbol ID matches the given one.
     */
    bool TokenReader::check_symbol_id(SymbolID symbol_id) const
    {
        return peek_symbol_id() == symbol_id;
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
     * Returns true if the current token's lexeme starts with the given prefix.
     */
    bool TokenReader::check_prefix(cl7::u8string_view prefix) const
    {
        return peek_token().lexeme.starts_with(prefix);
    }



    /**
     * Advances to the next token if the current token's symbol ID matches the given
     * one. Returns true if advanced, false otherwise.
     */
    bool TokenReader::skip_symbol_id(SymbolID symbol_id)
    {
        if (check_symbol_id(symbol_id))
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
