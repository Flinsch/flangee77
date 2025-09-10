#ifndef DL7_SYNTAX_BUFFEREDTOKENREADER_H
#define DL7_SYNTAX_BUFFEREDTOKENREADER_H
#include "./TokenReader.h"

#include <vector>



namespace dl7::syntax {



/**
 * A token reader that retrieves tokens from a pre-generated sequence. This
 * implementation stores tokens in an internal buffer (such as an `std::vector`)
 * and allows sequential reading with lookahead support.
 */
template <class Tcontainer = std::vector<Token>>
class BufferedTokenReader
    : public TokenReader
{

public:
    using container_type = Tcontainer;
    using iterator = typename container_type::const_iterator;



    explicit BufferedTokenReader(const container_type& tokens)
        : _tokens(tokens)
        , _it(_tokens.begin())
    {
        assert(!_tokens.empty());
    }

    explicit BufferedTokenReader(container_type&& tokens)
        : _tokens(std::move(tokens))
        , _it(_tokens.begin())
    {
        assert(!_tokens.empty());
    }

    ~BufferedTokenReader() override = default;



    /**
     * Returns the current token without advancing the reader.
     */
    const Token& peek_token() const override
    {
        if (_it == _tokens.end())
            return _invalid_eof_token;
        return *_it;
    }

    /**
     * Advances the reader and returns the next token.
     */
    const Token& next_token() override
    {
        if (_it == _tokens.end())
            return _invalid_eof_token;
        ++_it;
        return peek_token();
    }

    /**
     * Checks if the reader has reached the end of the token stream
     * (i.e., there are no more tokens or the "EOF" symbol has been reached).
     */
    bool is_eof() const override
    {
        if (_it == _tokens.end())
            return true;
        return _it->symbol_id == EOF_SYMBOL_ID;
    }



private:
    container_type _tokens;
    iterator _it;

    Token _invalid_eof_token = {.symbol_id = EOF_SYMBOL_ID};

}; // class BufferedTokenReader



} // namespace dl7::syntax

#endif // DL7_SYNTAX_BUFFEREDTOKENREADER_H
