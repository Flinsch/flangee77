#ifndef DL7_SYNTAX_GENERICLEXER_H
#define DL7_SYNTAX_GENERICLEXER_H
#include "./Lexer.h"

#include "./TerminalSymbolCollection.h"



namespace dl7::syntax {



/**
 * A generic lexer that tokenizes source text based on terminal symbol definitions.
 * It provides a generic implementation to transform a source text into a sequence
 * of recognized tokens suitable for various parser types.
 *
 * Applies the longest-match rule and prioritizes literals over non-literals (e.g.,
 * patterns). Otherwise, "earlier" symbols win over "later" ones added to the
 * terminal symbol collection.
 */
class GenericLexer
    : public Lexer
{

public:
    using Lexer::Lexer;

    GenericLexer(Diagnostics* diagnostics, const TerminalSymbolCollection* terminal_symbols, Options options);



private:
    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the character length of the recognized lexeme of the first/next
     * token at the very beginning of the source text.
     */
    std::pair<SymbolId, size_t> _recognize(cl7::u8string_view source) override;

    /**
     * Tries to match (the beginning of) the specified source text against the given
     * terminal symbol and returns the number of characters matched (or 0).
     */
    static size_t _try_match_next(cl7::u8string_view source, const TerminalSymbol& symbol);

    /**
     * The underlying collection of terminal symbols.
     */
    const TerminalSymbolCollection* _terminal_symbols;

}; // class GenericLexer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GENERICLEXER_H
