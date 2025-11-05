#ifndef DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
#define DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H

#include "./ProductionRule.h"

#include <vector>



namespace dl7::syntax {



/**
 * A collection of non-terminal symbols and their production rules for context-free,
 * BNF-like grammars. A non-terminal symbol can be defined multiple times to
 * represent the aspect of choices in the grammar.
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
    void add(SymbolId symbol_id, const std::vector<SymbolId>& sequence);

    /**
     * Adds the specified production rule the collection.
     */
    void add(SymbolId symbol_id, std::vector<SymbolId>&& sequence);

    /**
     * Returns the number of production rules in the collection.
     */
    size_t get_count() const { return _production_rules.size(); }

    /**
     * Retrieves the production rule at the specified index.
     */
    const ProductionRule& get(size_t index) const { assert(index < _production_rules.size()); return _production_rules[index]; }

    /**
     * Checks whether the assumed non-terminal symbol is defined by any production
     * rule in the collection.
     */
    bool is_defined(SymbolId symbol_id) const;

    /**
     * Checks whether the specified symbol is used in any production rule.
     */
    bool is_used(SymbolId symbol_id) const;



private:
    /** The "list" of production rules. */
    std::vector<ProductionRule> _production_rules;

}; // class ProductionRuleCollection



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
