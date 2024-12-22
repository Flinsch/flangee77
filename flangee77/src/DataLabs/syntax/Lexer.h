#pragma once
#ifndef DL7_SYNTAX_LEXER_H
#define DL7_SYNTAX_LEXER_H

#include "./SourceLocation.h"
#include "./TerminalSymbol.h"
#include "./Token.h"

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::syntax {



class Lexer
{

public:
    enum class WhitespaceHandling
    {
        Discard,
        Perserve,
    };

    struct Options
    {
        WhitespaceHandling whitespace_handling = WhitespaceHandling::Discard;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Lexer() = delete;

    Lexer(Options options);

    Lexer(const Lexer&) = delete;
    Lexer& operator = (const Lexer&) = delete;
    Lexer(Lexer&&) = delete;
    Lexer& operator = (Lexer&&) = delete;

    virtual ~Lexer() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Initializes this lexer with the specified source text and resets all internal
     * data, especially the current cursor position.
     */
    void init(cl7::u8string_view source);

    /**
     * Performs a lexical analysis of the source text at the current cursor position
     * and returns the next token after the cursor has been moved forward.
     */
    Token next();

    /**
     * Initializes this lexer with the specified source text, performs a lexical
     * analysis of it, and returns a "list" of recognized tokens.
     */
    std::vector<Token> tokenize(cl7::u8string_view source);



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the options.
     */
    const Options& get_options() const { return _options; }

    /**
     * Returns the source text.
     */
    cl7::u8string_view get_source() const { return _source; }

    /**
     * Returns the rest of the source text that has not yet been analyzed, depending
     * on the current cursor position.
     */
    cl7::u8string_view get_remainder() const { return _source.substr(_source_location.offset); }

    /**
     * Returns the current source location of the next character/token to be
     * recognized.
     */
    const SourceLocation& get_source_location() const { return _source_location; }



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the lexeme of the first/next token at the very beginning.
     */
    virtual std::pair<TerminalSymbol::ID, size_t> _recognize(cl7::u8string_view source) = 0;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Skips any whitespace characters and moves the cursor position forward if
     * necessary. Returns the number of whitespace characters skipped.
     */
    size_t _skip_whitespace();

    /**
     * Advances the current cursor position by the specified number of characters.
     */
    void _advance(size_t count);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The options.
     */
    const Options _options;

    /**
     * The source text.
     */
    cl7::u8string_view _source;

    /**
     * The current source location of the next character/token to be recognized.
     */
    SourceLocation _source_location;

}; // class Lexer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_LEXER_H
