#include "Lexer.h"

#include <CoreLabs/strings/inspect.h>



namespace dl7::syntax {



    Lexer::Lexer(Diagnostics* diagnostics, Options options)
        : ParseStage(diagnostics)
        , _options(options)
    {
    }



    /**
     * Initializes this lexer with the specified source text and resets all internal
     * data, especially the current cursor position.
     */
    void Lexer::init(cl7::u8string_view source)
    {
        _source = source;
        _source_offset = 0;
    }

    /**
     * Performs a lexical analysis of the source text at the current cursor position
     * and advances the cursor accordingly. Returns the recognized token.
     */
    Token Lexer::next_token()
    {
        // Keep or skip whitespace?
        if (_options.whitespace_handling == WhitespaceHandling::Discard)
            _skip_whitespace();

        if (_source_offset >= _source.length())
        {
            // Return "EOF" token.
            return {
                .symbol_id = EOF_SYMBOL_ID,
                .lexeme = cl7::u8string_view(),
                .source_offset = _source_offset,
            };
        }

        // (Try to) recognize next token.
        const auto [symbol_id, length] = _recognize(get_remainder());

        assert(symbol_id != EOF_SYMBOL_ID);
        assert(length > 0);
        assert(length + _source_offset <= _source.length());

        // Capture token ...
        Token token{
            .symbol_id = symbol_id,
            .lexeme = _source.substr(_source_offset, length),
            .source_offset = _source_offset,
        };

        // Move cursor forward and handle different line endings.
        _advance(length);

        // ... and return.
        return token;
    }

    /**
     * Initializes this lexer with the specified source text, performs a lexical
     * analysis of it, and returns a sequence of recognized tokens.
     */
    std::vector<Token> Lexer::tokenize(cl7::u8string_view source)
    {
        std::vector<Token> tokens;

        init(source);
        do
            tokens.push_back(next_token());
        while (tokens.back().symbol_id != EOF_SYMBOL_ID);

        return tokens;
    }



    /**
     * Skips any whitespace characters and moves the cursor position forward if
     * necessary. Returns the number of whitespace characters skipped.
     */
    size_t Lexer::_skip_whitespace()
    {
        const size_t length = cl7::strings::inspect::count_whitespace_prefix(get_remainder());
        assert(length + _source_offset <= _source.length());
        _advance(length);
        return length;
    }

    /**
     * Advances the current cursor position by the specified number of characters.
     */
    void Lexer::_advance(size_t count)
    {
        _source_offset += count;
    }



} // namespace dl7::syntax
