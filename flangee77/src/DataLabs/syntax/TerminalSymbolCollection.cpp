#include "TerminalSymbolCollection.h"

#include <algorithm>



namespace dl7::syntax {



    /**
     * Adds the specified terminal symbol to the collection.
     */
    void TerminalSymbolCollection::add(std::unique_ptr<TerminalSymbol> terminal_symbol)
    {
        _terminal_symbols.emplace_back(std::move(terminal_symbol));
    }

    /**
     * Adds a literal terminal symbol to the collection.
     */
    void TerminalSymbolCollection::add_literal(SymbolId id, cl7::u8string_view literal)
    {
        add(std::make_unique<LiteralSymbol>(id, literal));
    }

    /**
     * Adds a pattern-based terminal symbol to the collection.
     */
    void TerminalSymbolCollection::add_pattern(SymbolId id, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
    {
        add(std::make_unique<PatternSymbol>(id, pattern, syntax_options, match_flags));
    }

    /**
     * Adds a pattern-based terminal symbol with a literal prefix for optimization.
     */
    void TerminalSymbolCollection::add_pattern(SymbolId id, cl7::u8string_view literal_prefix, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
    {
        add(std::make_unique<PatternSymbol>(id, literal_prefix, pattern, syntax_options, match_flags));
    }

    /**
     * Checks whether a terminal symbol with the given ID is defined in the collection.
     */
    bool TerminalSymbolCollection::is_defined(SymbolId id) const
    {
        return std::ranges::any_of(_terminal_symbols, [id](const auto& terminal_symbol_ptr) {
            return terminal_symbol_ptr->id == id;
        });
    }



} // namespace dl7::syntax
