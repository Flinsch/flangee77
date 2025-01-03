#ifndef DL7_SYNTAX_GRAMMAR_H
#define DL7_SYNTAX_GRAMMAR_H

#include "./TerminalSymbolCollection.h"
#include "./ProductionRuleCollection.h"



namespace dl7::syntax {



/**
 * A BNF-like grammar with terminal symbols and their lexical compositions,
 * non-terminal symbols and their production sequences, and a start symbol.
 */
struct Grammar
{

    TerminalSymbolCollection terminal_symbols;
    ProductionRuleCollection production_rules;
    SymbolID start_symbol_id = -1;

}; // struct Grammar



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GRAMMAR_H
