#include "Lexer.h"

#include <CoreLabs/strings.h>



namespace dl7::syntax {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Lexer::Lexer(Options options)
        : _options(options) {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Initializes this lexer with the specified source text and resets all internal
     * data, especially the current cursor position.
     */
    void Lexer::init(cl7::u8string_view source)
    {
        _source = source;
        _source_location = SourceLocation();
    }

    /**
     * Performs a lexical analysis of the source text at the current cursor position
     * and returns the next token after the cursor has been moved forward.
     */
    Token Lexer::next()
    {
        // Keep or skip whitespace?
        if (_options.whitespace_handling == WhitespaceHandling::Discard)
            _skip_whitespace();

        if (_source_location.offset >= _source.length())
        {
            // Return "EOF" token.
            return {
                .symbol_id = 0,
                .lexeme = cl7::u8string_view(),
                .source_location = _source_location,
            };
        }

        // (Try to) recognize next token.
        const auto [symbol_id, length] = _recognize(get_remainder());

        assert(symbol_id != 0);
        assert(length > 0);
        assert(length + _source_location.offset <= _source.length());

        // Capture token ...
        Token token{
            .symbol_id = symbol_id,
            .lexeme = _source.substr(_source_location.offset, length),
            .source_location = _source_location,
        };

        // Move cursor forward and handle different line endings.
        _advance(length);

        // ... and return.
        return token;
    }

    /**
     * Initializes this lexer with the specified source text, performs a lexical
     * analysis of it, and returns a "list" of recognized tokens.
     */
    std::vector<Token> Lexer::tokenize(cl7::u8string_view source)
    {
        std::vector<Token> tokens;

        init(source);
        do
            tokens.push_back(next());
        while (tokens.back().symbol_id != 0);

        return tokens;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Skips any whitespace characters and moves the cursor position forward if
     * necessary. Returns the number of whitespace characters skipped.
     */
    size_t Lexer::_skip_whitespace()
    {
        const size_t length = cl7::strings::count_whitespace_prefix(get_remainder());
        assert(length + _source_location.offset <= _source.length());
        _advance(length);
        return length;
    }

    /**
     * Advances the current cursor position by the specified number of characters.
     */
    void Lexer::_advance(size_t count)
    {
        for (size_t i = 0; i < count; ++i, ++_source_location.offset)
        {
            assert(_source_location.offset < _source.length());
            const size_t line_break = cl7::strings::is_line_break_prefix(_source.substr(_source_location.offset));
            if (line_break > 0)
            {
                // Line break: advance the line, resetting the column.
                ++_source_location.line;
                _source_location.column = 1;

                if (line_break > 1)
                    _source_location.offset += line_break - 1; // Skip second character as well.
            }
            else
            {
                // "Regular" character: advance the column.
                ++_source_location.column;
            }
        } // for ...
    }



} // namespace dl7::syntax
