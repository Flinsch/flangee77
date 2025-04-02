#ifndef DL7_SYNTAX_PRODUCTIONRULE_H
#define DL7_SYNTAX_PRODUCTIONRULE_H

#include "./SymbolID.h"

#include <vector>



namespace dl7::syntax {



/**
 * Represents a production rule in a context-free, BNF-like grammar. A production
 * rule defines how a non-terminal symbol can be expanded into a sequence of symbols.
 */
struct ProductionRule
{

    /**
     * The non-terminal symbol ID (left-hand side of the production rule).
     * This symbol is defined by this rule and can be expanded using the sequence.
     */
    SymbolID symbol_id;

    /**
     * The sequence of terminal and/or non-terminal symbols (right-hand side of the production rule).
     * This specifies how the left-hand side symbol can be rewritten in the parsing process.
     */
    std::vector<SymbolID> sequence;



    /**
     * Checks whether the specified symbol is used in the sequence of this
     * production rule.
     */
    bool is_used(SymbolID symbol_id) const;

}; // struct ProductionRule



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PRODUCTIONRULE_H
