#ifndef DL7_SYNTAX_GRAMMAR_H
#define DL7_SYNTAX_GRAMMAR_H

#include "./TerminalSymbolCollection.h"
#include "./ProductionRuleCollection.h"



namespace dl7::syntax {



/**
 * A context-free, BNF-like grammar with terminal symbols and their lexical
 * compositions, non-terminal symbols defined by production rules and their
 * expansion sequences, and a start symbol as the entry point for parsing.
 */
struct Grammar
{

    /** The collection of terminal symbols and their lexical compositions. */
    TerminalSymbolCollection terminal_symbols;
    /** The collection of production rules that define non-terminal symbols. */
    ProductionRuleCollection production_rules;
    /** The start symbol ID, representing the entry point of the grammar (defaults to -1, indicating an uninitialized or undefined start symbol). */
    SymbolId start_symbol_id = -1;

}; // struct Grammar



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GRAMMAR_H
