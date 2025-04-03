#ifndef DL7_SYNTAX_LEXINGTOKENREADER_H
#define DL7_SYNTAX_LEXINGTOKENREADER_H
#include "./TokenReader.h"

#include "./Lexer.h"



namespace dl7::syntax {



/**
 * A token reader that retrieves tokens dynamically using a lexer. This
 * implementation calls a lexer to produce tokens on demand. This is useful when
 * token generation should be performed incrementally instead of being buffered.
 */
class LexingTokenReader
    : public TokenReader
{

public:
    explicit LexingTokenReader(Lexer* lexer)
        : _lexer(lexer)
    {
        assert(_lexer);
        _current_token = _lexer->next_token();
    }

    ~LexingTokenReader() override = default;



    /**
     * Returns the current token without advancing the reader.
     */
    const Token& peek_token() const override
    {
        return _current_token;
    }

    /**
     * Advances the reader and returns the next token.
     */
    const Token& next_token() override
    {
        _current_token = _lexer->next_token();
        return peek_token();
    }

    /**
     * Checks if the reader has reached the end of the token stream.
     */
    bool is_eof() const override
    {
        return _current_token.symbol_id == EOF_SYMBOL_ID;
    }



private:
    Lexer* _lexer;
    Token _current_token;

}; // class LexingTokenReader



} // namespace dl7::syntax

#endif // DL7_SYNTAX_LEXINGTOKENREADER_H
