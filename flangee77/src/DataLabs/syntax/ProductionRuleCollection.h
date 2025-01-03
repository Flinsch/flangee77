#ifndef DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
#define DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H

#include "./ProductionRule.h"

#include <vector>



namespace dl7::syntax {



/**
 * A "set" of non-terminal symbols and their production rules for BNF-like grammars.
 * A symbol can be defined multiple times, to represent the aspect of choices.
 */
class ProductionRuleCollection
{

public:
    using const_iterator = std::vector<ProductionRule>::const_iterator;

    const_iterator begin() const { return {_production_rules.cbegin()}; }
    const_iterator end() const { return {_production_rules.cend()}; }



    /**
     * Adds the specified production rule the collection.
     */
    void add(ProductionRule production_rule);

    /**
     * Adds the specified production rule the collection.
     */
    void add(SymbolID symbol_id, const std::vector<SymbolID>& sequence);

    /**
     * Adds the specified production rule the collection.
     */
    void add(SymbolID symbol_id, std::vector<SymbolID>&& sequence);

    /**
     * Returns the number of contained production rules.
     */
    size_t get_count() const { return _production_rules.size(); }

    /**
     * Returns the production rule identified by the given index.
     */
    const ProductionRule& get(size_t index) const { assert(index < _production_rules.size()); return _production_rules[index]; }

    /**
     * Checks whether the assumed non-terminal symbol is defined by any production
     * rule.
     */
    bool is_defined(SymbolID symbol_id) const;

    /**
     * Checks whether the specified symbol is used in any production rule.
     */
    bool is_used(SymbolID symbol_id) const;



private:
    /** The "list" of production rules. */
    std::vector<ProductionRule> _production_rules;

}; // class ProductionRuleCollection



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
