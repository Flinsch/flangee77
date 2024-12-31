#ifndef DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H
#define DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H

#include "./TerminalSymbol.h"

#include <vector>



namespace dl7::syntax {



class TerminalSymbolCollection
{

public:
    /**
     * Adds the specified terminal symbol to the collection.
     */
    void add(std::unique_ptr<TerminalSymbol> terminal_symbol)
    {
        _terminal_symbols.emplace_back(std::move(terminal_symbol));
    }

    /**
     * Adds the specified terminal symbol to the collection.
     */
    template <class TTerminalSymbol, class... Args>
        requires(std::derived_from<TTerminalSymbol, TerminalSymbol>)
    void add(cl7::u8string_view name, TerminalSymbol::ID id, Args&&... args)
    {
        add(std::make_unique<TTerminalSymbol>(name, id, std::forward<Args>(args)...));
    }

    /**
     * Adds the specified literal symbol to the collection.
     */
    void add_literal(cl7::u8string_view name, TerminalSymbol::ID id, cl7::u8string_view literal)
    {
        add(std::make_unique<LiteralSymbol>(name, id, literal));
    }

    /**
     * Adds the specified pattern symbol to the collection.
     */
    void add_pattern(cl7::u8string_view name, TerminalSymbol::ID id, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default)
    {
        add(std::make_unique<PatternSymbol>(name, id, pattern, syntax_options, match_flags));
    }

    /**
     * Adds the specified pattern symbol to the collection.
     */
    void add_pattern(cl7::u8string_view name, TerminalSymbol::ID id, std::string_view pattern, cl7::u8string_view literal_prefix, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default)
    {
        add(std::make_unique<PatternSymbol>(name, id, pattern, literal_prefix, syntax_options, match_flags));
    }

    /**
     * Returns the number of contained terminal symbols.
     */
    size_t get_count() const
    {
        return _terminal_symbols.size();
    }

    /**
     * Returns the terminal symbol identified by the given index.
     */
    const TerminalSymbol& get(size_t index) const
    {
        return *_terminal_symbols[index];
    }



private:
    /** The "list" of terminal symbols. */
    std::vector<std::unique_ptr<TerminalSymbol>> _terminal_symbols;

}; // class TerminalSymbolCollection



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H
