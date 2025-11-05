#ifndef DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H
#define DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H

#include "./TerminalSymbol.h"

#include <CoreLabs/iterators.h>

#include <vector>



namespace dl7::syntax {



/**
 * A collection of terminal symbols and their lexical compositions for use in lexers
 * and/or context-free, BNF-like grammars. A terminal symbol can be defined multiple
 * times, for example to resolve complex regular expressions or to replace those
 * patterns (or custom symbols) with literal variants.
 */
class TerminalSymbolCollection
{

public:
    using const_iterator = cl7::const_ptr_forward_iterator<TerminalSymbol>;

    const_iterator begin() const { return {_terminal_symbols.cbegin()}; }
    const_iterator end() const { return {_terminal_symbols.cend()}; }



    /**
     * Adds the specified terminal symbol to the collection.
     */
    void add(std::unique_ptr<TerminalSymbol> terminal_symbol);

    /**
     * Adds a terminal symbol of the specified type to the collection.
     */
    template <class TTerminalSymbol, class... Args>
        requires(std::derived_from<TTerminalSymbol, TerminalSymbol>)
    void add(SymbolId id, Args&&... args)
    {
        add(std::make_unique<TTerminalSymbol>(id, std::forward<Args>(args)...));
    }

    /**
     * Adds a literal terminal symbol to the collection.
     */
    void add_literal(SymbolId id, cl7::u8string_view literal);

    /**
     * Adds a pattern-based terminal symbol to the collection.
     */
    void add_pattern(SymbolId id, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);

    /**
     * Adds a pattern-based terminal symbol with a literal prefix for optimization.
     */
    void add_pattern(SymbolId id, cl7::u8string_view literal_prefix, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);

    /**
     * Adds a custom terminal symbol using a user-defined matching function.
     */
    template <PrefixMatcher TPrefixMatcher>
    void add_custom(SymbolId id, CustomSymbol<TPrefixMatcher>::PrefixMatcher matcher)
    {
        add(std::make_unique<CustomSymbol<TPrefixMatcher>>(id, matcher));
    }

    /**
     * Adds a custom terminal symbol using a user-defined matching function.
     */
    template <PrefixMatcher TPrefixMatcher>
    void add_custom(SymbolId id, TPrefixMatcher matcher)
    {
        add(std::make_unique<CustomSymbol<TPrefixMatcher>>(id, matcher));
    }

    /**
     * Returns the number of terminal symbols in the collection.
     */
    size_t get_count() const { return _terminal_symbols.size(); }

    /**
     * Retrieves the terminal symbol at the specified index.
     */
    const TerminalSymbol& get(size_t index) const { assert(index < _terminal_symbols.size()); return *_terminal_symbols[index]; }

    /**
     * Checks whether a terminal symbol with the given ID is defined in the collection.
     */
    bool is_defined(SymbolId id) const;



private:
    /** The "list" of terminal symbols. */
    std::vector<std::unique_ptr<TerminalSymbol>> _terminal_symbols;

}; // class TerminalSymbolCollection



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TERMINALSYMBOLCOLLECTION_H
