#ifndef DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
#define DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H

#include "./ProductionRule.h"

#include <vector>



namespace dl7::syntax {



class ProductionRuleCollection
{

public:
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



private:
    /** The "list" of production rules. */
    std::vector<ProductionRule> _production_rules;

}; // class ProductionRuleCollection



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PRODUCTIONRULECOLLECTION_H
