#ifndef DL7_SYNTAX_GRAMMARANALYZER_H
#define DL7_SYNTAX_GRAMMARANALYZER_H

#include "./Grammar.h"



namespace dl7::syntax {



struct GrammarAnalyzer
{

    /**
     * Checks the given grammar for inconsistencies and contradictions.
     */
    static bool validate(const Grammar& grammar);

}; // struct GrammarAnalyzer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GRAMMARANALYZER_H
