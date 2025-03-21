#ifndef DL7_SYNTAX_PRODUCTIONRULE_H
#define DL7_SYNTAX_PRODUCTIONRULE_H

#include "./SymbolID.h"

#include <vector>



namespace dl7::syntax {



struct ProductionRule
{

    /** The non-terminal symbol ID (left-hand side of the production rule). */
    SymbolID symbol_id;

    /** The sequence of terminal/non-terminal symbols (right-hand side of the production rule). */
    std::vector<SymbolID> sequence;



    /**
     * Checks whether the specified symbol is used in the sequence of this
     * production rule.
     */
    bool is_used(SymbolID symbol_id) const;

}; // struct ProductionRule



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PRODUCTIONRULE_H
