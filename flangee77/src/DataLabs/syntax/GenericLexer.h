#ifndef DL7_SYNTAX_GENERICLEXER_H
#define DL7_SYNTAX_GENERICLEXER_H
#include "./Lexer.h"

#include "./TerminalSymbolCollection.h"



namespace dl7::syntax {



class GenericLexer
    : public Lexer
{

public:
    GenericLexer() = delete;

    GenericLexer(const TerminalSymbolCollection* terminal_symbols, Options options);

    GenericLexer(const GenericLexer&) = delete;
    GenericLexer& operator=(const GenericLexer&) = delete;
    GenericLexer(GenericLexer&&) = delete;
    GenericLexer& operator=(GenericLexer&&) = delete;

    ~GenericLexer() override = default;



private:
    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the lexeme of the first/next token at the very beginning.
     */
    std::pair<TerminalSymbol::ID, size_t> _recognize(cl7::u8string_view source) override;

    /**
     * Tries to match (the beginning of) the specified source text against the given
     * terminal symbol and returns the number of characters matched (or 0).
     */
    static size_t _try_match(cl7::u8string_view source, const TerminalSymbol& symbol);

    /**
     * The underlying "set" of terminal symbols.
     */
    const TerminalSymbolCollection* _terminal_symbols;

}; // class GenericLexer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GENERICLEXER_H
