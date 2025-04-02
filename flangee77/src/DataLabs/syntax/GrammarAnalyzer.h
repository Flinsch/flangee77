#ifndef DL7_SYNTAX_GRAMMARANALYZER_H
#define DL7_SYNTAX_GRAMMARANALYZER_H

#include "./Grammar.h"

#include <CoreLabs/logging/LogContext.h>



namespace dl7::syntax {



/**
 * Provides analysis tools for context-free, BNF-like grammars: helps detect
 * inconsistencies, contradictions, or structural issues within a grammar.
 */
class GrammarAnalyzer
{

public:
    GrammarAnalyzer() = default;
    explicit GrammarAnalyzer(cl7::logging::LogContext log_context) : _log_context(log_context) {}



    /**
     * Validates the given grammar for inconsistencies and contradictions.
     */
    bool validate(const Grammar& grammar) const;



private:
    cl7::logging::LogContext _log_context;

}; // class GrammarAnalyzer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_GRAMMARANALYZER_H
