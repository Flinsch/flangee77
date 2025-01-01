#include "ProductionRuleCollection.h"



namespace dl7::syntax {



    /**
     * Adds the specified production rule the collection.
     */
    void ProductionRuleCollection::add(ProductionRule production_rule)
    {
        _production_rules.emplace_back(std::move(production_rule));
    }

    /**
     * Adds the specified production rule the collection.
     */
    void ProductionRuleCollection::add(SymbolID symbol_id, const std::vector<SymbolID>& sequence)
    {
        add({symbol_id, sequence});
    }

    /**
     * Adds the specified production rule the collection.
     */
    void ProductionRuleCollection::add(SymbolID symbol_id, std::vector<SymbolID>&& sequence)
    {
        add({symbol_id, std::move(sequence)});
    }



} // namespace dl7::syntax
