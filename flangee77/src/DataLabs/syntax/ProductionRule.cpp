#include "ProductionRule.h"



namespace dl7::syntax {



    /**
     * Checks whether the specified symbol is used in the sequence of this
     * production rule.
     */
    bool ProductionRule::is_used(SymbolID symbol_id) const
    {
        return std::ranges::find(sequence, symbol_id) != sequence.end();
    }



} // namespace dl7::syntax
