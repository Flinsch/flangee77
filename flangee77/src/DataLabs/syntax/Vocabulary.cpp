#include "Vocabulary.h"



namespace dl7::syntax {



    /**
     * Adds the name of a symbol to the vocabulary.
     */
    void Vocabulary::add_symbol(SymbolID symbol_id, cl7::u8string_view symbol_name)
    {
        assert(symbol_id > 0);
        assert(!symbol_name.empty());
        assert(_names_by_id.find(symbol_id) == _names_by_id.end());
        auto [it, inserted] = _names_by_id.emplace(symbol_id, cl7::u8string(symbol_name));
        assert(inserted);
    }

    /**
     * Searches for the name of the specified symbol and returns it (or an empty
     * string if not found).
     */
    cl7::u8string_view Vocabulary::get_symbol_name(SymbolID symbol_id) const
    {
        auto it = _names_by_id.find(symbol_id);
        if (it == _names_by_id.end())
            return {};
        return it->second;
    }



} // namespace dl7::syntax
