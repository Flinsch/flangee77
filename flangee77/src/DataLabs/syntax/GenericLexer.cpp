#include "GenericLexer.h"



namespace dl7::syntax {



    /**
     * Explicit constructor.
     */
    GenericLexer::GenericLexer(const TerminalSymbolCollection* terminal_symbols, Options options)
        : Lexer(options)
        , _terminal_symbols(terminal_symbols)
    {
        assert(_terminal_symbols);
    }



    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the lexeme of the first/next token at the very beginning.
     */
    std::pair<SymbolID, size_t> GenericLexer::_recognize(cl7::u8string_view source)
    {
        assert(!source.empty());

        std::pair<SymbolID, size_t> best_match = {-1, 1};
        bool best_is_literal = false;

        for (const auto& symbol : *_terminal_symbols)
        {
            const size_t length = _try_match(source, symbol);

            if (length == 0)
                continue;

            // Apply longest-match rule and prioritize literals over non-literals (i.e., patterns).
            if (best_match.first < 0 || length > best_match.second || (length == best_match.second && symbol.is_literal() && !best_is_literal))
            {
                best_match = {symbol.id, length};
                best_is_literal = symbol.is_literal();
            }
        } // for each terminal symbol

        return best_match;
    }

    /**
     * Tries to match (the beginning of) the specified source text against the given
     * terminal symbol and returns the number of characters matched (or 0).
     */
    size_t GenericLexer::_try_match(cl7::u8string_view source, const TerminalSymbol& symbol)
    {
        return symbol.try_match_prefix(source);
    }



} // namespace dl7::syntax
