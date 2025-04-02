#include "GrammarAnalyzer.h"

#include <CoreLabs/logging.h>



namespace dl7::syntax {



    /**
     * Validates the given grammar for inconsistencies and contradictions.
     */
    bool GrammarAnalyzer::validate(const Grammar& grammar) const
    {
        bool is_valid = true;

        if (grammar.terminal_symbols.get_count() == 0)
        {
            TRY_LOG_ERROR(_log_context, u8"Invalid grammar: no terminal symbols provided.");
            is_valid = false;
        }

        if (grammar.production_rules.get_count() == 0)
        {
            TRY_LOG_ERROR(_log_context, u8"Invalid grammar: no production rules provided.");
            is_valid = false;
        }

        if (grammar.start_symbol_id <= 0)
        {
            TRY_LOG_ERROR(_log_context, u8"Invalid grammar: no start symbol provided.");
            is_valid = false;
        }

        for (const auto& terminal_symbol : grammar.terminal_symbols)
        {
            bool is_symbol_used = false;

            for (const auto& production_rule : grammar.production_rules)
            {
                if (production_rule.symbol_id == terminal_symbol.id)
                {
                    TRY_LOG_ERROR(_log_context, u8"Invalid grammar: a symbol must be either terminal or non-terminal, not both (id: " + cl7::to_string(terminal_symbol.id) + u8").");
                    is_valid = false;
                }

                is_symbol_used |= production_rule.is_used(terminal_symbol.id);
            } // for each production rule

            if (!is_symbol_used)
            {
                TRY_LOG_WARNING(_log_context, u8"Inconsistent grammar: a terminal symbol is not used in any production rule (id: " + cl7::to_string(terminal_symbol.id) + u8").");
            }
        } // for each terminal symbol

        return is_valid;
    }



} // namespace dl7::syntax
