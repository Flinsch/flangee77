#include "ProductionRuleCollection.h"

#include <algorithm>



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
        add({.symbol_id=symbol_id, .sequence=sequence});
    }

    /**
     * Adds the specified production rule the collection.
     */
    void ProductionRuleCollection::add(SymbolID symbol_id, std::vector<SymbolID>&& sequence)
    {
        add({.symbol_id=symbol_id, .sequence=std::move(sequence)});
    }

    /**
     * Checks whether the assumed non-terminal symbol is defined by any production
     * rule.
     */
    bool ProductionRuleCollection::is_defined(SymbolID symbol_id) const
    {
        return std::ranges::any_of(_production_rules, [symbol_id](const auto& production_rule) {
            return production_rule.symbol_id == symbol_id;
        });
    }

    /**
     * Checks whether the specified symbol is used in any production rule.
     */
    bool ProductionRuleCollection::is_used(SymbolID symbol_id) const
    {
        return std::ranges::any_of(_production_rules, [symbol_id](const auto& production_rule) {
            return production_rule.is_used(symbol_id);
        });
    }



} // namespace dl7::syntax
