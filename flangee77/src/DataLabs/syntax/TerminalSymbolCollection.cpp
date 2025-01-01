#include "TerminalSymbolCollection.h"



namespace dl7::syntax {



    /**
     * Adds the specified terminal symbol to the collection.
     */
    void TerminalSymbolCollection::add(std::unique_ptr<TerminalSymbol> terminal_symbol)
    {
        _terminal_symbols.emplace_back(std::move(terminal_symbol));
    }

    /**
     * Adds the specified literal symbol to the collection.
     */
    void TerminalSymbolCollection::add_literal(SymbolID id, cl7::u8string_view literal)
    {
        add(std::make_unique<LiteralSymbol>(id, literal));
    }

    /**
     * Adds the specified pattern symbol to the collection.
     */
    void TerminalSymbolCollection::add_pattern(SymbolID id, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
    {
        add(std::make_unique<PatternSymbol>(id, pattern, syntax_options, match_flags));
    }

    /**
     * Adds the specified pattern symbol to the collection.
     */
    void TerminalSymbolCollection::add_pattern(SymbolID id, std::string_view pattern, cl7::u8string_view literal_prefix, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
    {
        add(std::make_unique<PatternSymbol>(id, pattern, literal_prefix, syntax_options, match_flags));
    }



} // namespace dl7::syntax
