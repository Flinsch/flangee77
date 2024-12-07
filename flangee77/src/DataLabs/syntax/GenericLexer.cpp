#include "GenericLexer.h"



namespace dl7 {
namespace syntax {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    GenericLexer::GenericLexer(const TerminalSymbolCollection& terminal_symbols, Options options)
        : Lexer( options )
        , _terminal_symbols( terminal_symbols )
    {
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Performs a lexical analysis of the specified source text and returns the
     * symbol ID and the lexeme of the first/next token at the very beginning.
     */
    std::pair<TerminalSymbol::ID, size_t> GenericLexer::_recognize(cl7::u8string_view source)
    {
        assert( source.length() > 0 );

        std::pair<TerminalSymbol::ID, size_t> best_match = { -1, 1 };
        bool best_is_literal = false;

        for ( size_t i = 0; i < _terminal_symbols.get_count(); ++i )
        {
            const TerminalSymbol& symbol = _terminal_symbols.get( i );
            const size_t length = _try_match( source, symbol );

            if ( length == 0 )
                continue;

            // Apply longest match rule and prioritize literals over non-literals (i.e., patterns).
            if ( best_match.first < 0 || length > best_match.second || (length == best_match.second && symbol.is_literal() && !best_is_literal) )
            {
                best_match = { symbol.id, length };
                best_is_literal = symbol.is_literal();
            }
        } // for each terminal symbol

        return best_match;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Tries to match (the beginning of) the specified source text against the given
     * terminal symbol and returns the number of characters matched (or 0).
     */
    size_t GenericLexer::_try_match(cl7::u8string_view source, const TerminalSymbol& symbol) const
    {
        return symbol.try_match_prefix( source );
    }



} // namespace syntax
} // namespace dl7
