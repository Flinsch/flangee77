#pragma once
#ifndef DL7_SYNTAX_GENERICLEXER_H
#define DL7_SYNTAX_GENERICLEXER_H
#include "./Lexer.h"

#include "./TerminalSymbolCollection.h"



namespace dl7 {
namespace syntax {



class GenericLexer
    : public Lexer
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    GenericLexer(const TerminalSymbolCollection& terminal_symbols, Options options);

    /**
     * Destructor.
     */
    virtual ~GenericLexer() = default;

private:
    /** Default constructor. */
    GenericLexer() = delete;
    /** Copy constructor. */
    GenericLexer(const GenericLexer&) = delete;
    /** Copy assignment operator. */
    GenericLexer& operator = (const GenericLexer&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The underlying "set" of terminal symbols.
     */
    const TerminalSymbolCollection& _terminal_symbols;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the lexeme of the first/next token at the very beginning.
     */
    virtual std::pair<TerminalSymbol::ID, size_t> _recognize(cl7::u8string_view source);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Tries to match (the beginning of) the specified source text against the given
     * terminal symbol and returns the number of characters matched (or 0).
     */
    size_t _try_match(cl7::u8string_view source, const TerminalSymbol& symbol) const;

}; // class GenericLexer



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_GENERICLEXER_H
