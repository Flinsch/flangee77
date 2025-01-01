#ifndef DL7_SYNTAX_VOCABULARY_H
#define DL7_SYNTAX_VOCABULARY_H

#include "./SymbolID.h"

#include <CoreLabs/string.h>

#include <unordered_map>



namespace dl7::syntax {



class Vocabulary
{

public:
    /**
     * Adds the name of a symbol to the vocabulary.
     */
    void add_symbol(SymbolID symbol_id, cl7::u8string_view symbol_name);

    /**
     * Searches for the name of the specified symbol and returns it (or an empty
     * string if not found).
     */
    cl7::u8string_view get_symbol_name(SymbolID symbol_id) const;

private:
    std::unordered_map<SymbolID, cl7::u8string> _names_by_id;

}; // class Vocabulary



} // namespace dl7::syntax

#endif // DL7_SYNTAX_VOCABULARY_H
